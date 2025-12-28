#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor("#569CD6"));
    keywordFormat.setFontWeight(QFont::Bold);

    const QStringList keywords = {
        "if", "else", "for", "while", "return",
        "break", "continue", "function", "var", "proto", "Sleep"
    };

    for (const QString &word : keywords) {
        rules.append({
            QRegularExpression("\\b" + word + "\\b"),
            keywordFormat
        });
    }
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor("#CE9178"));
    rules.append({
        QRegularExpression("\".*?\""),
        stringFormat
    });

    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor("#6A9955"));

    rules.append({
        QRegularExpression("//[^\n]*"),
        commentFormat
    });

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor("#B5CEA8"));

    rules.append({
        QRegularExpression("\\b\\d+\\b"),
        numberFormat
    });
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const Rule &rule : rules) {
        auto it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            auto match = it.next();
            setFormat(match.capturedStart(),
                      match.capturedLength(),
                      rule.format);
        }
    }
}
