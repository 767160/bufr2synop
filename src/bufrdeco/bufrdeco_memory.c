/***************************************************************************
 *   Copyright (C) 2013-2022 by Guillermo Ballester Valor                  *
 *   gbv@ogimet.com                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*!
 \file bufrdeco_memory.c
 \brief This file has the memory stufs for library bufrdeco
*/
#include "bufrdeco.h"

/*!
  \fn int bufrdeco_init_tables ( struct bufr_tables **t )
  \brief Init a struct \ref bufr_tables allocating space
  \param t pointer to the target pointer to struct \ref bufr_tables
  \return 0 if succeeded, 1 otherwise
*/
int bufrdeco_init_tables ( struct bufr_tables **t )
{
  bufrdeco_assert ( t != NULL );
  
  if ( *t != NULL )
    free ( ( void * ) *t );

  if ( ( *t = ( struct bufr_tables * ) calloc ( 1, sizeof ( struct bufr_tables ) ) ) == NULL )
    return 1;
  return 0;
}


/*!
  \fn int bufrdeco_free_tables ( struct bufr_tables **t )
  \brief Frees the allocated space for a struct \ref bufr_tables
  \param t pointer to the target pointer to struct \ref bufr_tables
  \return 0 and \a *t is set to NULL
*/
int bufrdeco_free_tables ( struct bufr_tables **t )
{
  bufrdeco_assert ( t != NULL );

  if ( *t != NULL )
    {
      free ( ( void * ) *t );
      *t = NULL;
    }
  return 0;
}

/*!
  \fn int bufrdeco_init_expanded_tree ( struct bufrdeco_expanded_tree **t )
  \brief Init a struct \ref bufrdeco_expanded_tree allocating space
  \param t pointer to the target pointer to struct \ref bufrdeco_expanded_tree

  \return 0 if succeeded, 1 otherwise
*/
int bufrdeco_init_expanded_tree ( struct bufrdeco_expanded_tree **t )
{
  bufrdeco_assert ( t != NULL );

  if ( *t != NULL )
    free ( ( void * ) *t );

  if ( ( *t = ( struct bufrdeco_expanded_tree * ) calloc ( 1, sizeof ( struct bufrdeco_expanded_tree ) ) ) == NULL )
    return 1;
  return 0;
}

/*!
  \fn int bufrdeco_free_expanded_tree ( struct bufrdeco_expanded_tree **t )
  \brief Frees the allocated space for a struct \ref bufrdeco_expanded_tree
  \param t pointer to the target pointer to struct \ref bufrdeco_expanded_tree

  \return 0 and \a *t is set to NULL
*/
int bufrdeco_free_expanded_tree ( struct bufrdeco_expanded_tree **t )
{
  bufrdeco_assert ( t != NULL );

  if ( *t != NULL )
    {
      free ( ( void * ) *t );
      *t = NULL;
    }
  return 0;
}

/*!
  \fn int bufrdeco_substitute_tables ( struct bufr_tables **replaced, struct bufr_tables *source, struct bufrdeco *b )
  \brief substitute an struct \ref bufr_tables into a struct \ref bufrdeco
  \param replaced  Pointer where to set the replaced pointer
  \param source Pointer to a struct \ref bufr_tables
  \param b pointer to the container basic struct \ref bufrdeco
  \return 0 if succeeded, 1 otherwise

  Remember that the struct \ref bufr_tables used in bufrdeco library is the one which pointer is in struct
  \ref bufrdeco . To avoid problems the struct must be initialized before substituted in this fucntion.
  Both source and replaced structs are not modified.

  This is useful if we do not want to read and parse tables again if the caller has a pool of
  already readed tables.
*/
int bufrdeco_substitute_tables ( struct bufr_tables **replaced, struct bufr_tables *source, struct bufrdeco *b )
{
  bufrdeco_assert ( b != NULL );
  
  *replaced = b->tables;
  if ( source == NULL )
    {
      // allocate memory for table
      return bufrdeco_init_tables ( & ( b->tables ) );
    }
  else
    b->tables = source;
  return 0;
}

/*!
   \fn int bufrdeco_init_subset_sequence_data ( struct bufrdeco_subset_sequence_data *ba )
   \brief Init a struct \ref bufrdeco_subset_sequence_data
   \param ba pointer to the target struct
   \return 0 if succeeded, 1 otherwise

   It is supossed that no memory is allocated for sequence. If we are not sure better use
   function \ref bufrdeco_clean_subset_sequence_data
*/
int bufrdeco_init_subset_sequence_data ( struct bufrdeco_subset_sequence_data *ba )
{
  bufrdeco_assert ( ba != NULL );
  
  memset ( ba, 0, sizeof ( struct bufrdeco_subset_sequence_data ) );
  if ( ( ba->sequence = ( struct bufr_atom_data * ) calloc ( 1, BUFR_NMAXSEQ * sizeof ( struct bufr_atom_data ) ) ) == NULL )
    {
      fprintf ( stderr,"%s():Cannot allocate memory for atom data array\n", __func__ );
      return 1;
    }
  ba->dim = BUFR_NMAXSEQ;
  return 0;
}

/*!
  \fn int bufrdeco_clean_subset_sequence_data ( struct bufrdeco_subset_sequence_data *ba )
  \brief Cleans a struct \ref bufrdeco_subset_sequence_data
  \param ba Pointer to struct \ref bufrdeco_subset_sequence_data to clean
  \return 0 if succeeded, 1 otherwise

  For eficience, if sequence in the struct \ref bufrdeco_subset_sequence_data is allocated, just set the used
  elements to zero. If is still no allocated memory for sequence inits the struct

*/
int bufrdeco_clean_subset_sequence_data ( struct bufrdeco_subset_sequence_data *ba )
{
  bufrdeco_assert ( ba != NULL );
  
  if ( ba->sequence != NULL )
    {
      ba->nd = 0;
      return 0;
    }
  else
    return bufrdeco_init_subset_sequence_data ( ba );
}

/*!
 \fn  int bufrdeco_free_subset_sequence_data ( struct bufrdeco_subset_sequence_data *ba )
 \brief Free the memory for sequence array in a struct \ref bufrdeco_subset_sequence_data
 \param ba pointer to the target struct to free
 \return 0 if succeeded
*/
int bufrdeco_free_subset_sequence_data ( struct bufrdeco_subset_sequence_data *ba )
{
  bufrdeco_assert ( ba != NULL );
  
  if ( ba->sequence != NULL )
    {
      free ( ( void * ) ba->sequence );
      ba->sequence = NULL;
    }
  return 0;
}

/*!
  \fn int bufrdeco_init_compressed_data_references ( struct bufrdeco_compressed_data_references *rf )
  \brief Init a struct bufrdeco_compressed_data_references
  \param rf pointer ti the target struct
  \return 0 if succeded, otherwise 1

  If already memory is allocated for array of references then just adjust the used index to zero. Otherwise
  it allocate the needed memory and init the struct

*/
int bufrdeco_init_compressed_data_references ( struct bufrdeco_compressed_data_references *rf )
{
  bufrdeco_assert ( rf != NULL );
  
  if ( rf->refs != NULL && rf->dim != 0 )
    {
      rf->nd = 0; // Here we set the used elements to 0 of dim
    }
  else if ( rf->refs == NULL )
    {
      // Here memory is still not allocated. Proceed to allocate with BUFR_NMAXSEQ
      if ( ( rf->refs = ( struct bufrdeco_compressed_ref * ) calloc ( 1, BUFR_NMAXSEQ * sizeof ( struct bufrdeco_compressed_ref ) ) ) == NULL )
        {
          fprintf ( stderr,"%s(): Cannot allocate memory for bufrdeco_compressed_ref array\n", __func__ );
          return 1;
        }
      rf->nd = 0; // Set de used elements (bufrdeco_compressed_ref)
      rf->dim = BUFR_NMAXSEQ; // Set de allocated bufr_compressed_rer elements
    }
  return 0;
}

/*!
  \fn int bufrdeco_clean_compressed_data_references ( struct bufrdeco_compressed_data_references *rf )
  \brief Clean a struct \ref bufrdeco_compressed_data_references
  \param rf pointer to the target struct \ref bufrdeco_compressed_data_references to clean
  \return If succeeded return 0, otherwise 1
*/
int bufrdeco_clean_compressed_data_references ( struct bufrdeco_compressed_data_references *rf )
{
  bufrdeco_assert ( rf != NULL );
  
  if ( rf->refs != NULL && rf->nd != 0 )
    rf->nd = 0;
  else
    return bufrdeco_init_compressed_data_references ( rf );
  return 0;
}

/*!
  \fn int bufrdeco_free_compressed_data_references ( struct bufrdeco_compressed_data_references *rf )
  \brief Free the memory allocated for array of references in a struct \ref bufrdeco_compressed_data_references
  \param rf pointer to the target struct \ref bufrdeco_compressed_data_references to free
  \return If succeeded return 0, otherwise 1
*/
int bufrdeco_free_compressed_data_references ( struct bufrdeco_compressed_data_references *rf )
{
  bufrdeco_assert ( rf != NULL );
  
  if ( rf->refs != NULL )
    {
      free ( ( void * ) rf->refs );
      rf->refs = NULL;
    }
  return 0;
}


/*!
 * \fn int bufrdeco_allocate_bitmap ( struct bufrdeco *b )
 * \brief allocate bitmap
 * \param b the active struct \ref bufrdeco
 * \return If succeeded return 0, otherwise 1
 */
int bufrdeco_allocate_bitmap ( struct bufrdeco *b )
{
  buf_t nba;
  
  bufrdeco_assert ( b != NULL );
    
  nba = b->bitmap.nba;

  if ( nba < BUFR_MAX_BITMAPS )
    {
      if ( b->bitmap.bmap[nba] != NULL )
        {
          // the bitmap already is allocated
          return 0;
        }
      // let's try to allocate it!
      if ( ( b->bitmap.bmap[nba] = ( struct bufrdeco_bitmap * ) calloc ( 1, sizeof ( struct bufrdeco_bitmap ) ) ) == NULL )
        {
          snprintf ( b->error, sizeof ( b->error ),"%s(): Cannot allocate space for struct bufrdeco_bitmap\n",  __func__ );
          return 1;
        }
      // Update de counter
      ( b->bitmap.nba )++;
      return 0;
    }
  else
    {
      snprintf ( b->error, sizeof ( b->error ),"%s(): Too much bitmaps to allocate. The limit is %d\n", __func__, BUFR_MAX_BITMAPS );
      return 1;
    }
}


/*!
 *  \fn int bufrdeco_clean_bitmaps ( struct bufrdeco *b )
 *  \brief Clean all allocated bitmaps, but still is in memory
 * \param b the active struct \ref bufrdeco
 * \return If succeeded return 0, otherwise 1
 */
int bufrdeco_clean_bitmaps ( struct bufrdeco *b )
{
  buf_t i;

  bufrdeco_assert ( b != NULL );
  
  for ( i = 0; i < b->bitmap.nba ; i++ )
    {
      if ( b->bitmap.bmap[i] == NULL )
        continue;
      memset ( b->bitmap.bmap[i], 0, sizeof ( struct bufrdeco_bitmap ) );
    }
  b->bitmap.nba = 0;
  return 0;
}


/*!
 * \fn int bufrdeco_free_bitmap_array ( struct bufrdeco_bitmap_array *a )
 * \brief Free an allocated bitmap array
 * \param a pointer to target struct \ref bufrdeco_bitmap_array to free
 * \return If succeeded return 0, otherwise 1
 */
int bufrdeco_free_bitmap_array ( struct bufrdeco_bitmap_array *a )
{
  buf_t i;

  bufrdeco_assert ( a != NULL );
  
  for ( i = 0; i < a->nba ; i++ )
    {
      if ( a->bmap[i] == NULL )
        continue;

      free ( ( void* ) a->bmap[i] );
      a->bmap[i] = NULL;
    }
  a->nba = 0;
  return 0;
}

