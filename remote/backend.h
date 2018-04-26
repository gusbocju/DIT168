#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class BackEnd : public QObject
{
    Q_OBJECT

public:
    explicit BackEnd(QObject *parent = nullptr);


signals:
    void moving();
public slots:
    void moveW();
    void moveA();
    void moveS();
    void moveD();
    void startV2V();
    void stopV2V();
    bool connection(int n);
    bool group1();
    bool group2();
    bool group3();
    bool group4();
    bool group5();
    bool group6();
    bool group7();
    bool group8();
    bool group9();
    bool group10();
    bool group11();
    bool group12();

};

#endif // BACKEND_H
