#include "PlcEdges.h"

/***************************************************************
 * Rutina      : R_TRIG()      Tipo        : Constructor
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - Ninguno
 * Descripción :
 *      Inicializa el detector de flanco ascendente en reposo.
 ***************************************************************/
R_TRIG::R_TRIG()
{
  q = false;
  previous_clk = false;
}

/***************************************************************
 * Rutina      : update()   * Tipo        : Método
 * Parámetros  :
 *       - clk : señal booleana de entrada.
 * 
 * Retorno     :
 *       - Ninguno
 * 
 * Descripción :
 *       Actualiza el estado interno del detector R_TRIG.
 * 
 *       Debe llamarse de forma cíclica desde loop().
 * **************************************************************/
void R_TRIG::update (bool clk)
{
  if (!previous_clk && clk)
  {
    q = true;
  }
  else
  {
    q = false;
  }
  previous_clk = clk;
}

/***************************************************************
 * Rutina      : Q()
 * Tipo        : Método de consulta
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - true  : se ha detectado flanco ascendente en este ciclo.
 *      - false : no se ha detectado flanco ascendente.
 * Descripción :
 *      Devuelve el estado actual de la salida Q.
 *
 *      Este método no modifica el estado interno del objeto.
 ***************************************************************/
bool R_TRIG::Q () const{
  return q;
}


/***************************************************************
 * Rutina      : F_TRIG()           Tipo        : Constructor
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - Ninguno
 * Descripción :
 *      Inicializa el detector de flanco descendente en reposo.
 ***************************************************************/
F_TRIG::F_TRIG()
{
  q = false;
  previous_clk = false;
}

/***************************************************************
 *  Rutina      : update()  Tipo        : Método
 *  Parámetros  :
 *       - clk : señal booleana de entrada.
 * 
 * Retorno     :
 *       - Ninguno
 * 
 * Descripción :
 *       Actualiza el estado interno del detector F_TRIG.
 * 
 *       Debe llamarse de forma cíclica desde loop().
 * **************************************************************/
void F_TRIG::update(bool clk)
{
  if(previous_clk && !clk)
  {
    q = true;
  }
  else
  {
    q = false;
  }
  previous_clk = clk;
}

/***************************************************************
 * Rutina      : Q()
 * Tipo        : Método de consulta
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - true  : se ha detectado flanco descendente en este ciclo.
 *      - false : no se ha detectado flanco descendente.
 * Descripción :
 *      Devuelve el estado actual de la salida Q.
 *
 *      Este método no modifica el estado interno del objeto.
 ***************************************************************/
bool F_TRIG::Q() const
{
  return q;
}