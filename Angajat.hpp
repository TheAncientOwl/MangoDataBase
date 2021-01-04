#pragma once

namespace App::Data
{
    class Angajat
    {
    private:
        std::string m_Nume;
        char* m_Prenume;
        float m_Salariu;
        int m_Varsta;

    public:
        //Constructori
        Angajat();
        Angajat(const std::string& nume, const char* prenume, float salariu, int varsta);
        Angajat(const Angajat& angajat);

        //Destructor
        ~Angajat();

        //Operatori
        Angajat& operator=(const Angajat& angajat);

        //Getteri
        const std::string& getNume() const;
        const char* getPrenume() const;
        float getSalariu() const;
        int getVarsta() const;

        //Setteri
        void setNume(const std::string& numeNou);
        void setPrenume(const char* prenumeNou);
        void setSalariu(float salariuNou);
        void setVarsta(int varstaNoua);
    };

    std::istream& operator>>(std::istream& in, Angajat& angajat);
    std::ostream& operator<<(std::ostream& out, Angajat& angajat);
}