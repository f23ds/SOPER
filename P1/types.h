/**
 * @file types.h
 * @author Fabio Desio and Álvaro de Santos
 * @brief enum to check solution
 * @version 0.1
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TYPES_H
#define TYPES_H

/* Nueva enum para guardar el estado de la solución computada por el minero */
typedef enum
{
  REJECTED, /* Solución declinada */
  ACCEPTED  /* Solución aceptada */
} STATUS;

#endif
