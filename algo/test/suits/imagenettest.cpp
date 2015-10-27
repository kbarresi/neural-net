#include "imagenettest.h"
#include "algo/test/neurontestinput.h"
#include "algo/neuron.h"

#include <QPixmap>
#include <QDir>
#include <QDebug>

ImageNetTest::ImageNetTest(int iterations) : TestSuit(iterations), DIMS(12) {
    m_inputLayer = nullptr;
    m_h1 = nullptr;
    m_outputLayer = nullptr;

    qDebug("Creating layers...");
    m_inputLayer = (NeuronTestInput***)(malloc(sizeof(NeuronTestInput***) * DIMS));
    m_h1 = (Neuron***)(malloc(sizeof(Neuron***) * DIMS));
    m_outputLayer = (Neuron***)(malloc(sizeof(Neuron***) * DIMS));
    for (int i = 0; i < DIMS; i++) {
        m_inputLayer[i] = (NeuronTestInput**)(malloc(sizeof(NeuronTestInput**) * DIMS));
        m_h1[i] = (Neuron**)(malloc(sizeof(Neuron**) * DIMS));
        m_outputLayer[i] = (Neuron**)(malloc(sizeof(Neuron**) * DIMS));

        for (int j = 0; j < DIMS; j++) {
            m_inputLayer[i][j] = new NeuronTestInput();
            m_h1[i][j] = new Neuron();
            m_outputLayer[i][j] = new Neuron();
        }
    }


    //Input -> H1
    qDebug("Connecting input to hidden 1...");
    for (int i = 0; i < DIMS; i++) {
        for (int j = 0; j < DIMS; j++) {
            Neuron* neuron = m_h1[i][j];
            for (int k = 0; k < DIMS; k++) {
                for (int m = 0; m < DIMS; m++)
                    neuron->addInput(m_inputLayer[k][m], 0.5f);
            }
        }
    }

    // H1 -> Output
    qDebug("Connecting hidden 1 to output...");
    for (int i = 0; i < DIMS; i++) {
        for (int j = 0; j < DIMS; j++) {
            Neuron* neuron = m_outputLayer[i][j];
            for (int k = 0; k < DIMS; k++) {
                for (int m = 0; m < DIMS; m++)
                    neuron->addInput(m_h1[k][m], 0.5f);
            }
        }
    }

}



ImageNetTest::~ImageNetTest() {

    for (int i = 0; i < DIMS; i++) {
        for (int j = 0; j < DIMS; j++) {
            free(m_inputLayer[i][j]);
            free(m_h1[i][j]);
            free(m_outputLayer[i][j]);
        }
        free(m_inputLayer[i]);
        free(m_h1[i]);
        free(m_outputLayer[i]);
    }
    free(m_inputLayer);
    free(m_h1);
    free(m_outputLayer);
}

void ImageNetTest::start() {
    //Load in each image to the input layer. Then go cycle through iterations.
    QString path = "/Users/kbarresi/Desktop/tr";
    QDir imageDir(path);
    QStringList imageFiles = imageDir.entryList(QDir::NoDotAndDotDot | QDir::Files);

    for (int imageIndex = 0; imageIndex < 20; imageIndex++) {
        QImage image = QImage(path + "/" + imageFiles[imageIndex]);
        QRect centerRect = QRect(0, 0, DIMS, DIMS);
        centerRect.setTopLeft(image.rect().center() - QPoint(DIMS/2, DIMS/2));
        image = image.copy(centerRect);

        for (int ii = 0; ii < image.height(); ii++) {
            uchar* scan = image.scanLine(ii);
            int depth =4;
            for (int jj = 0; jj < image.width(); jj++) {

                QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
                int gray = qGray(*rgbpixel);
                m_inputLayer[ii][jj]->setActivity(255.f / (float)gray);
            }
        }

        qDebug("Image %d", imageIndex);

        //24 FPS on the video, so thats 40 ms each frame (40 cycles)
        for (int iteration = 0; iteration < 40; iteration++) {
            for (int x = 0; x < DIMS; x++) {
                for (int y = 0; y < DIMS; y++) {
                    m_h1[x][y]->runCycle();
                }
            }

            for (int x = 0; x < DIMS; x++) {
                for (int y = 0; y < DIMS; y++) {
                    m_outputLayer[x][y]->runCycle();
                }
            }

            qDebug("\t%d finished...", iteration);
        }

    }

}

void ImageNetTest::printResults() const {

    qDebug("\n\n");
    for (int i = 0; i < DIMS; i++) {
        QString row;
        for (int j = 0; j < DIMS; j++) {
            row += QString("%1,").arg(QString::number(m_outputLayer[i][j]->activity()));
        }
        qDebug() << row;
    }
}
