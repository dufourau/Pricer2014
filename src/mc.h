#ifndef _MC_H
#define _MC_H

#include "option.h"
#include "bs.h"
#include "pnl/pnl_random.h"

class MonteCarlo
{
public:
  BS *mod_; /*! pointeur vers le modèle */
  Option *opt_; /*! pointeur sur l'option */
  PnlRng *rng; /*! pointeur sur le générateur */
  double h_; /*! pas de différence finie */
  int samples_; /*! nombre de tirages Monte Carlo */

  /**
   * Calcule le prix de l'option à la date 0
   *
   * @param[out] prix valeur de l'estimateur Monte Carlo
   * @param[out] ic largeur de l'intervalle de confiance
   */
  void price(double &prix, double &ic);

  /**
   * Calcule le prix de l'option à la date t
   *
   * @param[in]  past contient la trajectoire du sous-jacent
   * jusqu'à l'instant t
   * @param[in] t date à laquelle le calcul est fait
   * @param[out] prix contient le prix
   * @param[out] ic contient la largeur de l'intervalle
   * de confiance sur le calcul du prix
   */
  void price(const PnlMat *past, double t, double &prix, double &ic);
};

#endif /* _MC_H */

