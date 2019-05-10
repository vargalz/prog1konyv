// frakablak.cpp
//
// Mandelbrot halmaz nagyító

#include "frakablak.h"

FrakAblak::FrakAblak(double a, double b, double c, double d,
                     int szelesseg, int iteraciosHatar, QWidget *parent)
                         : QMainWindow(parent)
{
    setWindowTitle("Mandelbrot halmaz");

    szamitasFut = true;
    x = y = mx = my = 0;
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->szelesseg = szelesseg;
    this->iteraciosHatar = iteraciosHatar;
    magassag = (int)(szelesseg * ((d-c)/(b-a)));

    setFixedSize(QSize(szelesseg, magassag));
    fraktal= new QImage(szelesseg, magassag, QImage::Format_RGB32);

    mandelbrot = new FrakSzal(a, b, c, d, szelesseg, magassag, iteraciosHatar, this);
    mandelbrot->start();
}

FrakAblak::~FrakAblak()
{
    delete fraktal;
    delete mandelbrot;
}

void FrakAblak::paintEvent(QPaintEvent*) {
    QPainter qpainter(this);
    qpainter.drawImage(0, 0, *fraktal);
    if(!szamitasFut) {
        qpainter.setPen(QPen(Qt::white, 1));
        qpainter.drawRect(x, y, mx, my);
        if(!zX.empty()) //excuse me
        {
            for(int i=0; i<zX.size(); i++)
            {
                qpainter.drawLine(zX[i],zY[i],zX2[i],zY2[i]);
            }
 
        }

    }
    qpainter.end();
}

void FrakAblak::mousePressEvent(QMouseEvent* event) {
    
    if (event->button() == Qt::LeftButton)
    {

        // A nagyítandó kijelölt területet bal felsõ sarka:
        x = event->x();
        y = event->y();
        mx = 0;
        my = 0;
    }
    else if(event->button() == Qt::RightButton)
    {
               double dx = (b-a)/szelesseg;
               double dy = (d-c)/magassag;
               double reC, imC, reZ, imZ, ujreZ, ujimZ;
 
               int iteracio = 0;
 
               reC = a+event->x()*dx;
               imC = d-event->y()*dy;
 
               reZ = 0;
               imZ = 0;
               iteracio = 0;
 
               while(reZ*reZ + imZ*imZ < 4 && iteracio < 255) {
                   // z_{n+1} = z_n * z_n + c
                   ujreZ = reZ*reZ - imZ*imZ + reC;
                   ujimZ = 2*reZ*imZ + imC;
                   zX.push_back((int)((reZ - a)/dx));
                   zY.push_back( (int)((d - imZ)/dy));
                   zX2.push_back((int)((ujreZ - a)/dx));
                   zY2.push_back((int)((d - ujimZ)/dy));
                   reZ = ujreZ;
                   imZ = ujimZ;
 
                   ++iteracio;
                }
    }

    update();
}

void FrakAblak::mouseMoveEvent(QMouseEvent* event) {

    // A nagyítandó kijelölt terület szélessége és magassága:
    mx = event->x() - x;
    my = mx; // négyzet alakú

    update();
}

void FrakAblak::mouseReleaseEvent(QMouseEvent* event) {

    if(szamitasFut)
        return;

    szamitasFut = true;

    double dx = (b-a)/szelesseg;
    double dy = (d-c)/magassag;

    double a = this->a+x*dx;
    double b = this->a+x*dx+mx*dx;
    double c = this->d-y*dy-my*dy;
    double d = this->d-y*dy;

    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;

    delete mandelbrot;
    mandelbrot = new FrakSzal(a, b, c, d, szelesseg, magassag, iteraciosHatar, this);
    mandelbrot->start();

    update();
}

void FrakAblak::keyPressEvent(QKeyEvent *event)
{

    if(szamitasFut)
        return;

    if (event->key() == Qt::Key_N)
        iteraciosHatar *= 2;
    szamitasFut = true;

    delete mandelbrot;
    mandelbrot = new FrakSzal(a, b, c, d, szelesseg, magassag, iteraciosHatar, this);
    mandelbrot->start();

}


void FrakAblak::vissza(int magassag, int *sor, int meret)
{
    for(int i=0; i<meret; ++i) {
        QRgb szin = qRgb(0, 255-sor[i], 0);
        fraktal->setPixel(i, magassag, szin);
    }
    update();
}

void FrakAblak::vissza(void)
{
    szamitasFut = false;
    x = y = mx = my = 0;
}

