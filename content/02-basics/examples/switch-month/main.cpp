#include <iostream>
#include <string>

int main() {
  int m{};
  std::cin >> m;
  std::string mois;
  switch( m ) {
    case 1:
      mois = "janvier";      // the slide writes m = ... (a bug)
      break;
    case 2: mois = "février"; break;
    case 3: mois = "mars"; break;
    // Autres cas non montrés
    case 12:
      mois = "décembre";
      break;
    default:
      std::cout << "numéro de mois inconnu\n";
  }
  std::cout << mois << "\n";
}
