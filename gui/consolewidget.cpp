#include "consolewidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCharFormat>

#include <QPushButton>
#include <QLineEdit>

ConsoleTextView::ConsoleTextView(QWidget *parent) : QPlainTextEdit(parent) {
	setReadOnly(true);
}



void ConsoleTextView::addLine(const QString &text, ConsoleTextType type) {
	QTextCharFormat format;
	format.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

	QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);

    cursor.insertText(text, format);
    cursor.insertBlock();

    /*QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
	setTextCursor(cursor);*/
}



ConsoleWidget::ConsoleWidget(QWidget *parent) : QWidget(parent) {
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(4, 4, 4, 4);
	setLayout(layout);

	// Add the text view
	textView_ = new ConsoleTextView;
	layout->addWidget(textView_);

	textView_->addLine("Hello, world!");

	// Add the input
	QHBoxLayout *inputLayout = new QHBoxLayout;
	// inputLayout->setContentsMargins(8, 0, 8, 0);
	QLineEdit *lineCommand = new QLineEdit;
	lineCommand->setPlaceholderText(tr("Type '?' for help"));
	//lineCommand->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

	QPushButton *buttonRun = new QPushButton(tr("Run"));
	lineCommand->setFixedHeight(buttonRun->sizeHint().height());

	inputLayout->addWidget(lineCommand);
	inputLayout->addWidget(buttonRun);
	layout->addLayout(inputLayout);
}