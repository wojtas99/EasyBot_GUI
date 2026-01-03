#include "AgentThread.h"
#include "../../proto_functions_client.h"

#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <string>

#include <locale>
#include <codecvt>

#pragma comment(lib, "winhttp.lib")

void AgentThread::run() {
    proto->clearMessages();
    while (!isInterruptionRequested()) {
        auto messages = proto->getMessages(100);
        proto->clearMessages();
        if (list) {
            for (const auto& name : names) {
                for (const auto& message : messages) {
                    if (name != message.name) {
                        std::string response = performRequest_GPT(message);
                        if (message.mode == Otc::MessageSay || message.mode == Otc::MessageWhisper || Otc::MessageYell) proto->talk(response);
                        if (message.mode == Otc::MessagePrivateFrom) proto->talkPrivate(Otc::MessagePrivateTo, message.name, response);
                        msleep(1000);
                        break;
                    }
                }
            }
        } else {
            for (const auto& name : names) {
                for (const auto& message : messages) {
                    if (name == message.name) {
                        std::string response = performRequest_GPT(message);
                        if (message.mode == Otc::MessageSay) proto->talk(response);
                        if (message.mode == Otc::MessagePrivateFrom) proto->talkPrivate(Otc::MessagePrivateTo, message.name, response);
                        msleep(1000);
                        break;
                    }
                }
            }
        }
        msleep(3000);
    }
}


std::string AgentThread::performRequest_GPT(MessageStruct message) {
    SetConsoleOutputCP(CP_UTF8);

    std::string responseString;

    // Helper to escape special chars for the JSON REQUEST
    auto escape_json = [](const std::string& s) {
        std::string out;
        for (char c : s) {
            if (c == '\"') out += "\\\"";
            else if (c == '\\') out += "\\\\"; // Added backslash escaping
            else if (c == '\n') out += "\\n";
            else if (c == '\r') out += "\\r";
            else if (c == '\t') out += "\\t";
            else out += c;
        }
        return out;
    };

    std::string headerClean = escape_json(header);
    std::string messageClean = escape_json(message.text);

    std::string jsonData = "{"
        "\"model\": \"" + model + "\","
        "\"messages\": [{\"role\": \"system\", \"content\": \"" + headerClean + "\"},"
        "{\"role\": \"user\", \"content\": \"" + messageClean + "\"}]"
    "}";

    std::cout << "[DEBUG] JSON to send: " << jsonData << std::endl;

    std::string apiKeyClean = apiKey;
    // Remove newlines from API Key if present
    apiKeyClean.erase(std::remove(apiKeyClean.begin(), apiKeyClean.end(), '\n'), apiKeyClean.end());
    apiKeyClean.erase(std::remove(apiKeyClean.begin(), apiKeyClean.end(), '\r'), apiKeyClean.end());

    std::string headers = "Content-Type: application/json\r\nAuthorization: Bearer " + apiKeyClean + "\r\n";

    // Convert Headers to Wide String
    int wlen = MultiByteToWideChar(CP_UTF8, 0, headers.c_str(), -1, nullptr, 0);
    std::wstring wHeaders(wlen - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, headers.c_str(), -1, &wHeaders[0], wlen - 1);

    // --- WinHTTP Setup ---
    HINTERNET hSession = WinHttpOpen(L"ChatGPT C++ Client/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) { std::cout << "[ERROR] WinHttpOpen failed" << std::endl; return "Error"; }

    HINTERNET hConnect = WinHttpConnect(hSession, L"api.openai.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) { std::cout << "[ERROR] WinHttpConnect failed" << std::endl; WinHttpCloseHandle(hSession); return "Error"; }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/v1/chat/completions", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) { std::cout << "[ERROR] WinHttpOpenRequest failed" << std::endl; WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return "Error"; }

    // Send Request
    BOOL bResults = WinHttpSendRequest(hRequest, wHeaders.c_str(), (DWORD)-1L, (LPVOID)jsonData.c_str(), (DWORD)jsonData.size(), (DWORD)jsonData.size(), 0);
    if (!bResults) { std::cout << "[ERROR] WinHttpSendRequest failed" << std::endl; WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return "Error"; }

    if (!WinHttpReceiveResponse(hRequest, NULL)) { std::cout << "[ERROR] WinHttpReceiveResponse failed" << std::endl; WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return "Error"; }

    // Read Response
    DWORD bytesAvailable = 0;
    do {
        if (!WinHttpQueryDataAvailable(hRequest, &bytesAvailable)) break;
        if (bytesAvailable == 0) break;
        std::vector<char> buffer(bytesAvailable);
        DWORD bytesRead = 0;
        if (!WinHttpReadData(hRequest, buffer.data(), bytesAvailable, &bytesRead)) break;
        responseString.append(buffer.data(), bytesRead);
    } while (bytesAvailable > 0);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    std::cout << "[DEBUG] Full response length: " << responseString.size() << std::endl;

    // --- FIXED PARSING LOGIC START ---

    // 1. Find "choices"
    size_t posChoices = responseString.find("\"choices\"");
    if (posChoices == std::string::npos) return "";

    // 2. Find "message" inside choices
    size_t posMessage = responseString.find("\"message\"", posChoices);
    if (posMessage == std::string::npos) return "";

    // 3. Find "content" key
    size_t posContentKey = responseString.find("\"content\"", posMessage);
    if (posContentKey == std::string::npos) return "";

    // 4. Find the opening quote for the VALUE.
    // We search starting after "content" to skip the key.
    // This handles cases like "content": "..." (with space) or "content":"..." (no space)
    size_t posOpenQuote = responseString.find("\"", posContentKey + 9);
    if (posOpenQuote == std::string::npos) return "";

    // The actual content starts one character after the quote
    size_t currentPos = posOpenQuote + 1;

    std::string content;
    bool escapeMode = false;

    // 5. Parse character by character to handle escaped quotes properly
    while (currentPos < responseString.size()) {
        char c = responseString[currentPos];

        if (escapeMode) {
            // Unescape common JSON characters
            if (c == 'n') content += '\n';
            else if (c == 'r') content += '\r';
            else if (c == 't') content += '\t';
            else if (c == '\"') content += '\"';
            else if (c == '\\') content += '\\';
            else {
                // Keep original for others
                content += c;
            }
            escapeMode = false;
        }
        else {
            if (c == '\\') {
                escapeMode = true;
            }
            else if (c == '\"') {
                // Found the closing quote
                break;
            }
            else {
                content += c;
            }
        }
        currentPos++;
    }
    std::cout << "[DEBUG] Extracted content: " << content << std::endl;

    return content;
}