#include <QCoreApplication>
#include <QWidget>
#include <QFileDialog>
#include <QApplication>
#include <QImage>
#include <QTextStream>

char rgbToASCII(int r, int g, int b)
{
    // Конвертация цвета в оттенки серого
    int grayValue = 0.2126 * r + 0.7152 * g + 0.0722 * b;

    // Массив символов ASCII для создания градации от темного до светлого
    const std::string asciiChars = "@%#*+=-:. ";

    // Вычисление соответствующего символа ASCII
    int charIndex = std::round(grayValue / 255.0 * (asciiChars.size() - 1));

    return asciiChars[charIndex];
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QApplication app(argc,argv);

    QString filePath=QFileDialog::getOpenFileName(nullptr,"Choice file","","Images (*.png* *.jpg* *.jpeg* *.bmp*)",nullptr,QFileDialog::ReadOnly);

    // Загрузка изображения
    QImage image(filePath);
    if (image.isNull())
    {
        qDebug() << "Failed to load image";
        return 1;
    }

    // Создание текстового файла для сохранения ASCII графики
    QString outputFilePath = QFileDialog::getSaveFileName(nullptr, "Save file", "", "Text files (*.txt)");
    if (outputFilePath.isEmpty())
    {
        qDebug() << "No output file selected";
        return 1;
    }

    // Открытие файла для записи
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open output file";
        return 1;
    }

    // Создание объекта QTextStream для записи данных в файл
    QTextStream textStream(&outputFile);

    // Установка ширины текстового документа равной ширине изображения
    int documentWidth = image.width();

    // Преобразование пикселей изображения в ASCII символы и запись в файл
    for (int y = 0; y < image.height(); y++)
    {
        for (int x = 0; x < image.width(); x++)
        {
            QColor color = image.pixelColor(x, y);
            char asciiChar = rgbToASCII(color.red(), color.green(), color.blue());
            textStream << asciiChar;
        }

        // Добавление пробелов в конец строки для заполнения ширины текстового документа
        for (int i = image.width(); i < documentWidth; i++)
        {
            textStream << " ";
        }

        textStream << "\n";
    }

    // Закрытие файла
    outputFile.close();

    qDebug() << "ASCII art has been saved to" << outputFilePath;

    app.quit();

    return app.exec();
}
