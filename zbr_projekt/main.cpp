#include <conio.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>               //M_PI
#include <SFML/Graphics.hpp>
#include <string>               //std::string, std::to_string   
#include <windows.h>            //sleep;

//Katy podejscia, definiuj¹ce wektor podejœæia,
double psi_pod, teta_pod;

//Parametry definiuj¹ce po³o¿enie mechanizmu.
int delta1, delta2, delta5;

//Rozleg³oœci geometryczne mechanizmu maszyny manipulacyjnej.
double l1, l2, l3, l4, l5, l6, e, d;

//Liczba punktów pomiêdzy punktem startowym i koñcowym na trajektorii.
int liczba_krokow_symulacji;

//Tablice wspolrzednych maszynowych.
double* q1;
double* q2;
double* q3;
double* q4;
double* q5;
double* q23;
double* q234;

//WskaŸniki do tablicy wspó³rzêdnych punktów trajektorii w danej osi.
double* trajekt_x;
double* trajekt_y;
double* trajekt_z;

//Punkt poczatkowy.
double X_r, Y_r, Z_r;
//Punkt koncowy.
double X_k, Y_k, Z_k;

//Metoda obliczaj¹ca wspó³rzêdne maszynowe dla danych danych wejœciowych.
void oblicz_wsporzedne_maszynowe(double X, double Y, double Z, double psi, double teta, int delta1, int delta2, int delta5, double l1, double l2, double l3,
    double l4, double l5, double l6, double e, double d, double& Fi1, double& Fi2, double& Fi3, double& Fi4, double& Fi5, double& Fi23, double& Fi234);

//Metody s³u¿¹ce do przeliczania wartoœci k¹ta z radianów na stopnie i odwrotnie.
double przelicz_na_rad(double kat);
double przelicz_na_kat(double kat);

//Algorytm s³u¿¹cy do uzyskania wartoœci wspó³rzêdnej maszynowej z jak najwiêksz¹
//dok³adnoœci¹ znaj¹c wartoœæ jej funkcji trygonometrycznych.
double wartosc_wspolrzednej(double cosf, double sinf);

//Metoda wyliczaj¹ca trajektoriê i wspó³rzêdne maszynowe dla punktów znajduj¹cych
//siê na niej.
void policz();

//Metody rysuj¹ce poszczególne rzuty.
void kresl_xy();


int main()
{
    //Pocz¹tkowe wartoœci zmiennych otrzymywanych od u¿ytkownika
    psi_pod = 30;
    teta_pod = 60;
    delta1 = -1;
    delta2 = -1;
    delta5 = -1;
    l1 = 200;
    l2 = 600;
    l3 = 600;
    l4 = 400;
    l5 = 300;
    l6 = 200;
    e = 100;
    d = 200;
    X_r = 100;
    Y_r = 400;
    Z_r = 100;
    X_k = -500;
    Y_k = 500;
    Z_k = 500;

    //Pocz¹tkowe parametry symulacji.
    liczba_krokow_symulacji = 30;

    //Zalokowanie pamiêci dynamicznej na zmienne w zale¿noœci od liczby punktów tworz¹cych trajektorie.
    q1 = new double[liczba_krokow_symulacji];
    q2 = new double[liczba_krokow_symulacji];
    q3 = new double[liczba_krokow_symulacji];
    q4 = new double[liczba_krokow_symulacji];
    q5 = new double[liczba_krokow_symulacji];
    q23 = new double[liczba_krokow_symulacji];
    q234 = new double[liczba_krokow_symulacji];

    trajekt_x = new double[liczba_krokow_symulacji];
    trajekt_y = new double[liczba_krokow_symulacji];
    trajekt_z = new double[liczba_krokow_symulacji];

    policz();
    kresl_xy();
}

//Implementacja metod przeliczaj¹cych wartoœci k¹tów z stopni na radiany i odwrotnie,
double przelicz_na_rad(double kat)
{
    kat = kat / 180 * M_PI;
    return kat;
}

double przelicz_na_kat(double kat)
{
    kat = kat * 180 / M_PI;
    return kat;
}

//Algorytm otrzymywania wartoœci wspó³rzêdnej maszynowej opisany
//bardziej szczegó³owo w dokumentacji do projektu.
double wartosc_wspolrzednej(double cosf, double sinf)
{
    double fi;
    if (abs(cosf) > abs(sinf))
    {
        fi = asin(sinf);
        if (cosf < 0)
            fi = M_PI - fi;
    }
    else {
        fi = acos(cosf);
        if (sinf < 0)
            fi = -fi;
    }
    return fi;
}

//Metoda dokonuj¹ca opisanych w dokumentacji obliczeñ - wyliczaj¹ca
//wartoœci wspó³rzêdnych maszynowych dla pojedynczego punktu.
void oblicz_wsporzedne_maszynowe(double X, double Y, double Z, double psi, double teta, int delta1, int delta2, int delta5, double l1, double l2, double l3, double l4,
    double l5, double l6, double e, double d, double& Fi1, double& Fi2, double& Fi3, double& Fi4, double& Fi5, double& Fi23, double& Fi234)
{
    double Xp, Yp, Zp;
    double l = l5 + l6;
    double sinFi1, cosFi1;
    double sinFi2, cosFi2;
    double sinFi3, cosFi3;
    double sinFi4, cosFi4;
    double sinFi5, cosFi5;
    double sinFi234, cosFi234;
    double sinFi23, cosFi23;
    double Xr, Yr, Zr;
    double a, b;

    //Wyznaczenie wspolrzednych punktu P.
    Xp = X - l * cos(teta) * cos(psi);
    Yp = Y - l * cos(teta) * sin(psi);
    Zp = Z - l * sin(teta);

    //Wyznaczenie pierwszej wspolrzednej maszynowej.
    sinFi1 = 1 / (Xp * Xp + Yp * Yp) * (e * Xp + delta1 * Yp * sqrt(Xp * Xp + Yp * Yp - e * e));
    cosFi1 = 1 / (Xp * Xp + Yp * Yp) * (-e * Yp + delta1 * Xp * sqrt(Xp * Xp + Yp * Yp - e * e));
    Fi1 = wartosc_wspolrzednej(cosFi1, sinFi1);

    //Wyznaczenie piatej wspolrzednej maszynowej.
    sinFi5 = cos(teta) * (sin(psi) * cosFi1 - cos(psi) * sinFi1);
    cosFi5 = delta5 * sqrt(1 - sinFi5 * sinFi5);
    Fi5 = wartosc_wspolrzednej(cosFi5, sinFi5);

    //Wyznaczenie sumy wspó³rzednych maszynowych 2, 3, 4.
    sinFi234 = sin(teta) / cosFi5;
    cosFi234 = cos(teta) * (cos(psi) * cosFi1 + sin(psi) * sinFi1) / cosFi5;
    Fi234 = wartosc_wspolrzednej(cosFi234, sinFi234);

    //Wyznaczenie wspolrzednych koñca czêœci regionalnej R.
    Xr = Xp - l4 * cosFi1 * cosFi234;
    Yr = Yp - l4 * sinFi1 * cosFi234;
    Zr = Zp - l4 * sinFi234;

    //Wyznaczenie parametrów pomocniczych a i b.
    a = -l1 + delta1 * sqrt(Xr * Xr + Yr * Yr - e * e);
    b = 1 / (2 * l2) * (a * a + Zr * Zr + l2 * l2 - l3 * l3);

    //Wyznaczenie 2 wspolrzednej maszynowej.
    sinFi2 = 1 / (a * a + Zr * Zr) * (Zr * b + delta2 * a * sqrt(a * a + Zr * Zr - b * b));
    cosFi2 = 1 / (a * a + Zr * Zr) * (a * b - delta2 * Zr * sqrt(a * a + Zr * Zr - b * b));
    Fi2 = wartosc_wspolrzednej(cosFi2, sinFi2);

    //Wyznaczenie 3 wspolrzednej maszynowej.
    sinFi3 = -delta2 / l3 * sqrt(a * a + Zr * Zr - b * b);
    cosFi3 = (b - l2) / l3;
    Fi3 = wartosc_wspolrzednej(cosFi3, sinFi3);

    //Wyznaczenie sumy 2 i 3 wspó³rzêdnej maszynowej.
    sinFi23 = 1 / l3 * (Zr - l2 * sinFi2);
    cosFi23 = 1 / l3 * (a - l2 * cosFi2);
    Fi23 = wartosc_wspolrzednej(cosFi23, sinFi23);

    //Wyznaczenie 4 wspolrzednej maszynowej.
    sinFi4 = sinFi234 * cosFi23 - cosFi234 * sinFi23;
    cosFi4 = cosFi234 * cosFi23 + sinFi234 * sinFi23;
    Fi4 = wartosc_wspolrzednej(cosFi4, sinFi4);

}

//Metoda dokonuj¹ca interpolacji liniowej i obliczenia wspó³rzêdnych maszynowych
//dla ka¿dego otrzymanego w ten sposób punktu trajektorii.
void policz()
{
    double teta = przelicz_na_rad(teta_pod);
    double psi = przelicz_na_rad(psi_pod);
    double dx, dy, dz;

    //Obliczenie przyrostów.
    dx = (X_k - X_r) / (liczba_krokow_symulacji - 1);
    dy = (Y_k - Y_r) / (liczba_krokow_symulacji - 1);
    dz = (Z_k - Z_r) / (liczba_krokow_symulacji - 1);

    //Obliczenie wspó³rzêdnych maszynowych dla kolejnych punktów.
    oblicz_wsporzedne_maszynowe(X_r, Y_r, Z_r, psi, teta, delta1, delta2, delta5, l1, l2, l3, l4, l5, l6, e, d, q1[0], q2[0], q3[0], q4[0], q5[0], q23[0], q234[0]);
    trajekt_x[0] = X_r;
    trajekt_y[0] = Y_r;
    trajekt_z[0] = Z_r;
    for (int i = 1; i < liczba_krokow_symulacji; i++)
    {

        trajekt_x[i] = X_r + i * dx;
        trajekt_y[i] = Y_r + i * dy;
        trajekt_z[i] = Z_r + i * dz;
        oblicz_wsporzedne_maszynowe(trajekt_x[i], trajekt_y[i], trajekt_z[i], psi, teta, delta1, delta2, delta5, l1, l2, l3, l4, l5, l6, e, d, q1[i],
            q2[i], q3[i], q4[i], q5[i], q23[i], q234[i]);

    }
}

//Algorytm rysowania rzutu XY. Dla osi Y znaki przyrostów s¹ odwrócone co wynika z
//domyœlnej orientacji uk³adu wspó³rzêdnych obrazów w openCV (oœ Y zwrócona w dó³ obrazu).

void kresl_xy()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "all_views");

    sf::Font font;
    font.loadFromFile("../resources/fonts/buon_umore/Buon Umore.ttf");

    for (int i = 0; i < liczba_krokow_symulacji; i++) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type)
            {
            case sf::Event::Closed: window.close(); break;
            default: break;
            }
        }

        window.clear();

        double S = 3;
        sf::Vector2f Punkt_nastepny, Punkt_poprzedni;
        
        //xy
        {
            Punkt_poprzedni.x = 480;
            Punkt_poprzedni.y = 270;

            //1
            sf::CircleShape circle1(8);
            sf::Rect<float> size1 = circle1.getGlobalBounds();
            circle1.setOrigin(size1.width / 2, size1.height / 2);
            circle1.setPosition(Punkt_poprzedni);
            sf::Texture texture1;
            texture1.loadFromFile("../resources/textures/1.png");
            circle1.setTexture(&texture1);

            sf::Text text1(std::to_string(int(przelicz_na_kat(q1[i]))), font, 24);
            text1.setPosition(Punkt_poprzedni);

            Punkt_nastepny.x = Punkt_poprzedni.x + int(l1 * cos(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l1 * sin(q1[i]) / S + 0.5);

            sf::Vertex line1[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //2
            sf::CircleShape circle2(8);
            sf::Rect<float> size2 = circle2.getGlobalBounds();
            circle2.setOrigin(size2.width / 2, size2.height / 2);
            circle2.setPosition(Punkt_nastepny);
            sf::Texture texture2;
            texture2.loadFromFile("../resources/textures/2.png");
            circle2.setTexture(&texture2);

            sf::Text text2(std::to_string(int(przelicz_na_kat(q2[i]))), font, 24);
            text2.setPosition(Punkt_nastepny);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(d * sin(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y + int(d * cos(q1[i]) / S + 0.5);;

            sf::Vertex line2[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(l2 * cos(q1[i]) * cos(q2[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l2 * sin(q1[i]) * cos(q2[i]) / S + 0.5);

            sf::Vertex line3[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x - int((d - e) * sin(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int((d - e) * cos(q1[i]) / S + 0.5);

            sf::Vertex line4[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //3
            sf::CircleShape circle3(8);
            sf::Rect<float> size3 = circle3.getGlobalBounds();
            circle3.setOrigin(size3.width / 2, size3.height / 2);
            circle3.setPosition(Punkt_nastepny);
            sf::Texture texture3;
            texture3.loadFromFile("../resources/textures/3.png");
            circle3.setTexture(&texture3);

            sf::Text text3(std::to_string(int(przelicz_na_kat(q3[i]))), font, 24);
            text3.setPosition(Punkt_nastepny);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(l3 * cos(q23[i]) * cos(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l3 * cos(q23[i]) * sin(q1[i]) / S + 0.5);

            sf::Vertex line5[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //4
            sf::CircleShape circle4(8);
            sf::Rect<float> size4 = circle4.getGlobalBounds();
            circle4.setOrigin(size4.width / 2, size4.height / 2);
            circle4.setPosition(Punkt_nastepny);
            sf::Texture texture4;
            texture4.loadFromFile("../resources/textures/4.png");
            circle4.setTexture(&texture4);

            sf::Text text4(std::to_string(int(przelicz_na_kat(q4[i]))), font, 24);
            text4.setPosition(Punkt_nastepny);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(l4 * cos(q234[i]) * cos(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l4 * cos(q234[i]) * sin(q1[i]) / S + 0.5);

            sf::Vertex line6[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //5
            sf::CircleShape circle5(8);
            sf::Rect<float> size5 = circle5.getGlobalBounds();
            circle5.setOrigin(size5.width / 2, size5.height / 2);
            circle5.setPosition(Punkt_nastepny);
            sf::Texture texture5;
            texture5.loadFromFile("../resources/textures/5.png");
            circle5.setTexture(&texture5);

            sf::Text text5(std::to_string(int(przelicz_na_kat(q5[i]))), font, 24);
            text5.setPosition(Punkt_nastepny);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(l5 * cos(przelicz_na_rad(teta_pod)) * cos(przelicz_na_rad(psi_pod)) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l5 * cos(przelicz_na_rad(teta_pod)) * sin(przelicz_na_rad(psi_pod)) / S + 0.5);

            sf::Vertex line7[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //6
            sf::CircleShape circle6(8);
            sf::Rect<float> size6 = circle6.getGlobalBounds();
            circle6.setOrigin(size6.width / 2, size6.height / 2);
            circle6.setPosition(Punkt_nastepny);
            sf::Texture texture6;
            texture6.loadFromFile("../resources/textures/6.png");
            circle6.setTexture(&texture6);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int((l6)*cos(przelicz_na_rad(teta_pod)) * cos(przelicz_na_rad(psi_pod)) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int((l6)*cos(przelicz_na_rad(teta_pod)) * sin(przelicz_na_rad(psi_pod)) / S + 0.5);

            sf::Vertex line8[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //7
            sf::CircleShape circle7(8);
            sf::Rect<float> size7 = circle7.getGlobalBounds();
            circle7.setOrigin(size7.width / 2, size7.height / 2);
            circle7.setPosition(Punkt_nastepny);
            sf::Texture texture7;
            texture7.loadFromFile("../resources/textures/7.png");
            circle7.setTexture(&texture7);

            window.draw(line1, 2, sf::Lines);
            window.draw(line2, 2, sf::Lines);
            window.draw(line3, 2, sf::Lines);
            window.draw(line4, 2, sf::Lines);
            window.draw(line5, 2, sf::Lines);
            window.draw(line6, 2, sf::Lines);
            window.draw(line7, 2, sf::Lines);
            window.draw(line8, 2, sf::Lines);

            window.draw(circle1);
            window.draw(circle2);
            window.draw(circle3);
            window.draw(circle4);
            window.draw(circle5);
            window.draw(circle6);
            window.draw(circle7);

            window.draw(text1);
            window.draw(text2);
            window.draw(text3);
            window.draw(text4);
            window.draw(text5);
        }

        //zy
        {
            Punkt_poprzedni.x = 1280;
            Punkt_poprzedni.y = 270;

            //1
            sf::CircleShape circle1(8);
            sf::Rect<float> size1 = circle1.getGlobalBounds();
            circle1.setOrigin(size1.width / 2, size1.height / 2);
            circle1.setPosition(Punkt_poprzedni);
            sf::Texture texture1;
            texture1.loadFromFile("../resources/textures/1.png");
            circle1.setTexture(&texture1);

            sf::Text text1(std::to_string(int(przelicz_na_kat(q1[i]))), font, 24);
            text1.setPosition(Punkt_poprzedni);

            Punkt_nastepny.x = Punkt_poprzedni.x + int(l1 * sin(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y;

            sf::Vertex line1[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //2
            sf::CircleShape circle2(8);
            sf::Rect<float> size2 = circle2.getGlobalBounds();
            circle2.setOrigin(size2.width / 2, size2.height / 2);
            circle2.setPosition(Punkt_nastepny);
            sf::Texture texture2;
            texture2.loadFromFile("../resources/textures/2.png");
            circle2.setTexture(&texture2);

            sf::Text text2(std::to_string(int(przelicz_na_kat(q2[i]))), font, 24);
            text2.setPosition(Punkt_nastepny);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x - int(d * cos(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y;

            sf::Vertex line2[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(l2 * sin(q1[i]) * cos(q2[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l2 * sin(q2[i]) / S + 0.5);

            sf::Vertex line3[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int((d - e) * cos(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y;

            sf::Vertex line4[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //3
            sf::CircleShape circle3(8);
            sf::Rect<float> size3 = circle3.getGlobalBounds();
            circle3.setOrigin(size3.width / 2, size3.height / 2);
            circle3.setPosition(Punkt_nastepny);
            sf::Texture texture3;
            texture3.loadFromFile("../resources/textures/3.png");
            circle3.setTexture(&texture3);

            sf::Text text3(std::to_string(int(przelicz_na_kat(q3[i]))), font, 24);
            text3.setPosition(Punkt_nastepny);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(l3 * cos(q23[i]) * sin(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l3 * sin(q23[i]) / S + 0.5);

            sf::Vertex line5[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //4
            sf::CircleShape circle4(8);
            sf::Rect<float> size4 = circle4.getGlobalBounds();
            circle4.setOrigin(size4.width / 2, size4.height / 2);
            circle4.setPosition(Punkt_nastepny);
            sf::Texture texture4;
            texture4.loadFromFile("../resources/textures/4.png");
            circle4.setTexture(&texture4);

            sf::Text text4(std::to_string(int(przelicz_na_kat(q4[i]))), font, 24);
            text4.setPosition(Punkt_nastepny);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(l4 * cos(q234[i]) * sin(q1[i]) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l4 * sin(q234[i]) / S + 0.5);

            sf::Vertex line6[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //5
            sf::CircleShape circle5(8);
            sf::Rect<float> size5 = circle5.getGlobalBounds();
            circle5.setOrigin(size5.width / 2, size5.height / 2);
            circle5.setPosition(Punkt_nastepny);
            sf::Texture texture5;
            texture5.loadFromFile("../resources/textures/5.png");
            circle5.setTexture(&texture5);

            sf::Text text5(std::to_string(int(przelicz_na_kat(q5[i]))), font, 24);
            text5.setPosition(Punkt_nastepny);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int(l5 * cos(przelicz_na_rad(teta_pod)) * sin(przelicz_na_rad(psi_pod)) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l5 * sin(przelicz_na_rad(teta_pod)) / S + 0.5);

            sf::Vertex line7[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //6
            sf::CircleShape circle6(8);
            sf::Rect<float> size6 = circle6.getGlobalBounds();
            circle6.setOrigin(size6.width / 2, size6.height / 2);
            circle6.setPosition(Punkt_nastepny);
            sf::Texture texture6;
            texture6.loadFromFile("../resources/textures/6.png");
            circle6.setTexture(&texture6);

            Punkt_poprzedni = Punkt_nastepny;
            Punkt_nastepny.x = Punkt_poprzedni.x + int((l6)*cos(przelicz_na_rad(teta_pod)) * sin(przelicz_na_rad(psi_pod)) / S + 0.5);
            Punkt_nastepny.y = Punkt_poprzedni.y - int(l6 * sin(przelicz_na_rad(teta_pod)) / S + 0.5);

            sf::Vertex line8[] =
            {
                sf::Vertex(Punkt_poprzedni),
                sf::Vertex(Punkt_nastepny)
            };
            //7
            sf::CircleShape circle7(8);
            sf::Rect<float> size7 = circle7.getGlobalBounds();
            circle7.setOrigin(size7.width / 2, size7.height / 2);
            circle7.setPosition(Punkt_nastepny);
            sf::Texture texture7;
            texture7.loadFromFile("../resources/textures/7.png");
            circle7.setTexture(&texture7);

            window.draw(line1, 2, sf::Lines);
            window.draw(line2, 2, sf::Lines);
            window.draw(line3, 2, sf::Lines);
            window.draw(line4, 2, sf::Lines);
            window.draw(line5, 2, sf::Lines);
            window.draw(line6, 2, sf::Lines);
            window.draw(line7, 2, sf::Lines);
            window.draw(line8, 2, sf::Lines);

            window.draw(circle1);
            window.draw(circle2);
            window.draw(circle3);
            window.draw(circle4);
            window.draw(circle5);
            window.draw(circle6);
            window.draw(circle7);

            window.draw(text1);
            window.draw(text2);
            window.draw(text3);
            window.draw(text4);
            window.draw(text5);
        }

        window.display();
        Sleep(300);
    }
}