#ifndef CONSOLE_WIDGET_H
#define CONSOLE_WIDGET_H

#include <QPlainTextEdit>

enum class ConsoleTextType {
	Normal,
	Warning,
	Fatal,
};

class ConsoleTextView : public QPlainTextEdit {
	Q_OBJECT
public:
	explicit ConsoleTextView(QWidget *parent = nullptr);

public slots:
	void addLine(const QString &text, ConsoleTextType type = ConsoleTextType::Normal);
};

class ConsoleWidget : public QWidget {
	Q_OBJECT
public:
	explicit ConsoleWidget(QWidget *parent = nullptr);

private:

	ConsoleTextView *textView_;
};

#endif