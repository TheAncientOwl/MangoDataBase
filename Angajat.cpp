#include "standard_library.hpp"
#include "Angajat.hpp"

namespace App::Data
{
    Angajat::Angajat()
        : m_Nume(""), m_Prenume(nullptr), m_Salariu(0), m_Varsta(0)
    {
    }

    Angajat::Angajat(const std::string& nume, const char* prenume, float salariu, int varsta)
    {
        this->setNume(nume);
        this->setPrenume(prenume);
        this->setSalariu(salariu);
        this->setVarsta(varsta);
    }

    Angajat::Angajat(const Angajat& angajat)
    {
        m_Nume = angajat.m_Nume;
        
        m_Prenume = new char[strlen(angajat.m_Prenume) + 1];
        strcpy_s(m_Prenume, strlen(angajat.m_Prenume) + 1, angajat.m_Prenume);

        m_Salariu = angajat.m_Salariu;
        m_Varsta = angajat.m_Varsta;
    }

    Angajat::~Angajat()
    {
        if (m_Prenume)
            delete[] m_Prenume;
    }

    Angajat& Angajat::operator=(const Angajat& angajat)
    {
        /// self-asignment
        if (this == &angajat)
            return *this;

        /// memory cleanup
        if (m_Prenume)
            delete[] m_Prenume;

        /// memory copy
        m_Nume = angajat.m_Nume;

        if (angajat.m_Prenume)
        {
            m_Prenume = new char[strlen(angajat.m_Prenume) + 1];
            strcpy_s(m_Prenume, strlen(angajat.m_Prenume) + 1, angajat.m_Prenume);
        }
        else m_Prenume = nullptr;

        m_Salariu = angajat.m_Salariu;
        m_Varsta = angajat.m_Varsta;

        /// return
        return *this;
    }

    const std::string& Angajat::getNume() const
    {
        return m_Nume;
    }

    const char* Angajat::getPrenume() const
    {
        return m_Prenume;
    }

    float Angajat::getSalariu() const
    {
        return m_Salariu;
    }

    int Angajat::getVarsta() const
    {
        return m_Varsta;
    }

    void Angajat::setNume(const std::string& numeNou)
    {
        if (numeNou.length() < 3)
            throw std::invalid_argument("Nume invalid");

        m_Nume = numeNou;
    }

    void Angajat::setPrenume(const char* prenumeNou)
    {
        if (!prenumeNou)
            throw std::invalid_argument("Numele nu poate fi nul");

        size_t lungimePrenume = strlen(prenumeNou);
        if (lungimePrenume < 3)
            throw std::invalid_argument("Nume invalid");

        if (m_Prenume)
            delete[] m_Prenume;

        m_Prenume = new char[lungimePrenume + 1];

        strcpy_s(m_Prenume, lungimePrenume + 1, prenumeNou);
    }

    void Angajat::setSalariu(float salariuNou)
    {
        if (salariuNou < 0)
            throw std::invalid_argument("Salariul nu poate fi negativ!");

        m_Salariu = salariuNou;
    }

    void Angajat::setVarsta(int varstaNoua)
    {
        if (varstaNoua < 0)
            throw std::invalid_argument("Varsta nu poate fi negativa!");

        m_Varsta = varstaNoua;
    }

    std::istream& operator>>(std::istream& in, Angajat& angajat)
    {
        bool success = true;

        ///string
        do
        {
            success = true;

            std::cout << "Nume: ";

            std::string aux;
            std::getline(in, aux);

            try
            {
                angajat.setNume(aux);
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << e.what() << '\n';
                success = false;
            }

        } while (!success);

        ///char*
        do
        {
            success = true;

            std::cout << "Prenume: ";

            std::string aux;
            std::getline(in, aux);

            try
            {
                angajat.setPrenume(aux.c_str());
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << e.what() << '\n';
                success = false;
            }

        } while (!success);

        ///float
        do
        {
            success = true;

            std::cout << "Salariu: ";

            float aux = 0;
            in >> aux;

            try
            {
                angajat.setSalariu(aux);
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << e.what() << '\n';
                success = false;
            }

        } while (!success);

        ///int
        do
        {
            success = true;

            std::cout << "Varsta: ";

            int aux = 0;
            in >> aux;

            try
            {
                angajat.setVarsta(aux);
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << e.what() << '\n';
                success = false;
            }

        } while (!success);

        return in;
    }

    std::ostream& operator<<(std::ostream& out, Angajat& angajat)
    {
        out << "Nume: " << angajat.getNume() << '\n';
        if (angajat.getPrenume())
            out << "Prenume: " << angajat.getPrenume() << '\n';
        out << "Salariu: " << angajat.getSalariu() << '\n';
        out << "Varsta: " << angajat.getVarsta() << '\n';

        return out;
    }

}