#include "standard_library.hpp"
///Hotel

class UUID//universal unique id
{

};

class Data
{
private:
    int zi;
    int luna;
    int an;
};

template<typename T>
class Registru
{
private:
    T* data;
    int numarInregistrari;
};

class Persoana
{
private:
    UUID id;
    std::string nume;
    std::string prenume;
    char* telefon;
    char* email;
    Data dataNastere;
    char sex;
};

class Angajat : Persoana
{
private:
    Data dataAngajare;
    float salariu;
};

class DateCazare
{
private:
    UUID id;
    Persoana* client;
    char* modalitatePlata;
    int numarZileCazare;
};

class Rezervare
{
private:
    UUID id;
    DateCazare* dateCazare;
    Data dataEfectuare;
};

class Cazare
{
private:
    UUID id;
    DateCazare* dateCazare;
    Angajat* angajat;
    Data dataEfectuare;
};

class CameraRezervata
{
private:
    UUID id;
    DateCazare* dateCazare;
};