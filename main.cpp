#include <QCoreApplication>
#include <QWidget>
#include <QFileDialog>
#include <QApplication>
#include <QImage>
#include <QTextStream>

char rgbToASCII(int r, int g, int b)
{
    // Convert colour to greyscale
    int grayValue = 0.2126 * r + 0.7152 * g + 0.0722 * b;

    // Array of ASCII characters to create dark to light gradation
    const std::string asciiChars = "@%#*+=-:. ";

    // Calculation of the corresponding ASCII character
    int charIndex = std::round(grayValue / 255.0 * (asciiChars.size() - 1));

    return asciiChars[charIndex];
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QApplication app(argc,argv);

    QString filePath=QFileDialog::getOpenFileName(nullptr,"Choice file","","Images (*.png* *.jpg* *.jpeg* *.bmp*)",nullptr,QFileDialog::ReadOnly);

    // Uploading an image
    QImage image(filePath);
    if (image.isNull())
    {
        qDebug() << "Failed to load image";
        return 1;
    }

    // Creating a text file to save ASCII graphics
    QString outputFilePath = QFileDialog::getSaveFileName(nullptr, "Save file", "", "Text files (*.txt)");
    if (outputFilePath.isEmpty())
    {
        qDebug() << "No output file selected";
        return 1;
    }

    // Opening a file for writing
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open output file";
        return 1;
    }

    // Create a QTextStream object to write data to a file
    QTextStream textStream(&outputFile);

    // Set the width of the text document equal to the width of the image
    int documentWidth = image.width();

    // Convert image pixels to ASCII characters and write to file
    for (int y = 0; y < image.height(); y++)
    {
        for (int x = 0; x < image.width(); x++)
        {
            QColor color = image.pixelColor(x, y);
            char asciiChar = rgbToASCII(color.red(), color.green(), color.blue());
            textStream << asciiChar;
        }

        // Add spaces at the end of a line to fill the width of a text document
        for (int i = image.width(); i < documentWidth; i++)
        {
            textStream << " ";
        }

        textStream << "\n";
    }

    // Closing the file
    outputFile.close();

    qDebug() << "ASCII art has been saved to" << outputFilePath;

    app.quit();

    return app.exec();
}
