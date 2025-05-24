#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <iterator>

// Functie de conversie FNC
std::set<std::string> conversieFNC(const std::set<std::string>& formule) {
    std::set<std::string> S;

    // Trecem prin fiecare formula din set
    for (const std::string& formula : formule) {
        if (formula[0] == '!') {
            // !(a->b)
            std::string subFormula = formula.substr(2); // Remove '!( '
            size_t pos = subFormula.find("->");

            if (pos != std::string::npos) {
                std::string stg = subFormula.substr(0, pos); // partea stanga (a)
                std::string dr = subFormula.substr(pos + 2, 1); // partea dreapta (b)

                // Adaugare 'a' si '!b' separat
                S.insert(stg);
                S.insert("n" + dr);
            }
        }

        else if (formula.find("->") != std::string::npos) {
            // impart prin '->' pentru a separa ce e in stanga si ce e in dreapta
            size_t pos = formula.find("->");
            std::string stg = formula.substr(0, pos); // partea stanga (a)
            std::string dr = formula.substr(pos + 2); // partea dreapta (b)

            //converteste a->b la !a V b
            S.insert("n" + stg + "V" + dr);
        }
        else {
            // daca nu avem nici implicatie, nici negatie, se adauga formula asa cum e
            S.insert(formula);
        }
    }

    return S;
}

// Functie pentru rezolvarea de clauze si verificare tautologie
std::set<std::string> rezolvareClauze(const std::set<std::string>& S, const std::string& clauza1, const std::string& clauza2) {
    std::set<std::string> clauzeNoi;

    // imparte clauza1 in 2 parti dupa 'V', cea din stanga V si cea din dreapta V
    std::string stg1, dr1;
    size_t pos1 = clauza1.find('V');
    if (pos1 != std::string::npos) {
        stg1 = clauza1.substr(0, pos1);
        dr1 = clauza1.substr(pos1 + 1);
    } else {
        stg1 = clauza1; // Single-literal clause
    }

    // imparte clauza2 in 2 parti dupa 'V', cea din stanga V si cea din dreapta V
    std::string stg2, dr2;
    size_t pos2 = clauza2.find('V');
    if (pos2 != std::string::npos) { // verifica daca se gaseste V in clauza
        stg2 = clauza2.substr(0, pos2);
        dr2 = clauza2.substr(pos2 + 1);
    } else {
        stg2 = clauza2; // daca V nu exista, se ia stringul ca un singur element
    }

    // Logica pentru principiul rezolutiei
    if (stg1 == "n" + stg2 || stg2 == "!" + stg1) {
        // verifica complementaritate intre partile stanga ale celor 2 clauze
        if (!dr1.empty()) clauzeNoi.insert(dr1);
        if (!dr2.empty()) clauzeNoi.insert(dr2);
    } else if (stg1 == "n" + dr2 || dr2 == "n" + stg1) {
        // Complementaritate intre stanga1 si dreapta2
        if (!dr1.empty()) clauzeNoi.insert(dr1 + "V" + stg2);
        else if (!stg2.empty()) clauzeNoi.insert(stg2);
    } else if (dr1 == "n" + stg2 || stg2 == "n" + dr1) {
        // Complementaritate intre stanga2 si dreapta1
        if (!stg1.empty()) clauzeNoi.insert(stg1 + "V" + dr2);
        else if (!dr2.empty()) clauzeNoi.insert(dr2);
    } else if (dr1 == "n" + dr2 || dr2 == "n" + dr1) {
        // verifica complementaritate intre partile dreapta ale celor 2 clauze
        if (!stg1.empty()) clauzeNoi.insert(stg1 + "V" + stg2);
        else if (!stg2.empty()) clauzeNoi.insert(stg2);
    }

    if (pos1 == std::string::npos && pos2 != std::string::npos) {
        // clauza1 e un singur caracter
        if (clauza1 == "n" + stg2 && !dr2.empty()) {
            clauzeNoi.insert(dr2); // partea ramasa din clauza2
        } else if (clauza1 == "n" + dr2 && !stg2.empty()) {
            clauzeNoi.insert(stg2); // partea ramasa din clauza2
        }
    } else if (pos2 == std::string::npos && pos1 != std::string::npos) {
        // clauza2 e un singur caracter
        if (clauza2 == "n" + stg1 && !dr1.empty()) {
            clauzeNoi.insert(dr1); // partea ramasa din clauza1
        } else if (clauza2 == "n" + dr1 && !stg1.empty()) {
            clauzeNoi.insert(stg1); // partea ramasa din clauza1
        }
    }

    // filtreaza tautologiile
    std::set<std::string> clauzeFiltr;
    for (const std::string& clauza : clauzeNoi) {

        size_t pos = clauza.find('V');
        if (pos != std::string::npos) {
            std::string stg = clauza.substr(0, pos);
            std::string dr = clauza.substr(pos + 1);
            if (stg == dr || stg == "n" + dr || dr == "n" + stg || stg.empty() || dr.empty()) {
                continue;
            }
        } else if (clauza.empty()) {
            continue;
        }

         if (S.find(clauza) == S.end()) {
            clauzeFiltr.insert(clauza);
        }
    }


    return clauzeFiltr;
}

void verifContrad(const std::set<std::string>& S) {
    for (const std::string& clauza : S) {

        if (clauza[0] != '!') {
            std::string negatie = "n" + clauza;
            if (S.find(negatie) != S.end()) {
                std::cout << "Contradictie: " << clauza << " si " << negatie << std::endl;
                std::cout << "Sistem inconsistent";
                return;
            }
        } else {
            std::string negatie = clauza.substr(1);
            if (S.find(negatie) != S.end()) {
                std::cout << "Contradictie: " << clauza << " si " << negatie << std::endl;
                std::cout << "Sistem inconsistent";
                return;
            }
        }

    }
    std::cout << "Sistem consistent";
}


int main() {
    // Sir de formule original
    std::string sirFormule = "p->q,q->r,r->s,s->t";

    // Set pentru stocarea formulelor unice
    std::set<std::string> formule;

    // Sir pentru impartirea sirului de formule dupa virgula
    std::stringstream ss(sirFormule);
    std::string formula;

    // Se extrag formulele separate de ','
    while (std::getline(ss, formula, ',')) {
        // Gasim formulele si taiem spatiile libere care sunt atat dupa, cat si inainte de formula in sine
        size_t start = formula.find_first_not_of(" \t");
        size_t end = formula.find_last_not_of(" \t");
        if (start != std::string::npos && end != std::string::npos) {
            formule.insert(formula.substr(start, end - start + 1));
        // Formula fara spatii este introdusa in setul de formule unice
        }
    }

    // Se face conversia formulelor la FNCuri
    std::set<std::string> S = conversieFNC(formule);
    std::cout << "Sistem de formule: " << sirFormule << std::endl;

    // Initializare S'
    std::set<std::string> Sprim;
    bool rezolutie = true;
    bool gasitCon = false; // steag pentru gasire contradictie

    // Cat timp rezolutia are loc
    while (rezolutie && !gasitCon) {
        rezolutie = false;
        // Se incearca rezolvarea de clauze
        for (auto it1 = S.begin(); it1 != S.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != S.end(); ++it2) {

                std::set<std::string> clauzeNoi = rezolvareClauze(S, *it1, *it2);

                // Daca are loc rezolutia, o adaugam la S'
                if (!clauzeNoi.empty()) {
                    // Verificam tautologiile si le ignoram
                    for (const auto& clauzaNoua : clauzeNoi) {
                        if (gasitCon) {
                            break;
                        }
                        rezolutie = true;
                        Sprim.insert(clauzaNoua);
                    }
                }
            }
        }

        if (!Sprim.empty()) {
            // Afisare S (formule curente)
            std::cout << "S curent (FNC):" << std::endl;
            for (const std::string& f : S) {
                std::cout << f << ", ";
            }
            std::cout << std::endl;

            // Afisare S'
            std::cout << "S': " << std::endl;
            for (const std::string& f : Sprim) {
                std::cout << f << ", ";
            }
            std::cout << std::endl;

            // Facem S reunit cu S' in S
            S.insert(Sprim.begin(), Sprim.end());

            // Resetam, stergem toate elementele din S'
            Sprim.clear();
        }
    }

    std::cout << "S:" << std::endl;
    for (const std::string& f : S) {
        std::cout << f << ", ";
    }
    std::cout << std::endl;

    // Verificam pentru contradictii in S
    verifContrad(S);

    return 0;
}
