#ifndef _BS_H
#define _BS_H

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Modèle de Black Scholes
class BS
{
public:
  int size_; /// nombre d'actifs du modèle
  double r_; /// taux d'intérêt
  double rho_; /// paramètre de corrélation
  PnlVect *sigma_; /// vecteur de volatilités
  PnlVect *spot_; /// valeurs initiales du sous-jacent

  /**
   * Génère une trajectoire du modèle et la stocke dans path
   *
   * @param[out] path contient une trajectoire du modèle.
   * C'est une matrice de taille d x (N+1)
   * @param[in] T  maturité
   * @param[in] N nombre de dates de constatation
   */
  void asset(PnlMat *path, double T, int N, PnlRng *rng);

  /**
   * Calcule une trajectoire du sous-jacent connaissant le
   * passé jusqu' à la date t
   *
   * @param[out] path  contient une trajectoire du sous-jacent
   * donnée jusqu'à l'instant t par la matrice past
   * @param[in] t date jusqu'à laquelle on connait la trajectoire
   * t n'est pas forcément une date de discrétisation
   * @param[in] N nombre de pas de constatation
   * @param[in] T date jusqu'à laquelle on simule la trajectoire
   * @param[in] past trajectoire réalisée jusqu'a la date t
   */
  void asset(PnlMat *path, double t, int N, double T,
             PnlRng *rng, const PnlMat *past);
  /**
   * Calcule le delta de l'option à la date t
   *
   * @param[in] past contient la trajectoire du sous-jacent
   * jusqu'à l'instant t
   * @param[in] t date à laquelle le calcul est fait
   * @param[out] delta contient le vecteur de delta
   * @param[out] ic contient la largeur de l'intervalle
   * de confiance sur le calcul du delta
   */
  void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic);

  /**
   * Shift d'une trajectoire du sous-jacent
   *
   * @param[in]  path contient en input la trajectoire
   * du sous-jacent
   * @param[out] shift_path contient la trajectoire path
   * dont la composante d a été shiftée par (1+h)
   * à partir de la date t.
   * @param[in] t date à partir de laquelle on shift
   * @param[in] h pas de différences finies
   * @param[in] d indice du sous-jacent à shifter
   * @param[in] timestep pas de constatation du sous-jacent
   */
  void shift_asset(PnlMat *shift_path, const PnlMat *path,
                   int d, double h, double t, double timestep);

};


#endif /* _BS_H */

