/** \file prioqueue.h
 * \brief defines the interface for a priority-queue / heap data-structure
 * \author Stefan Naumann
 * \date 2014
 **/

#ifndef MST_PRIO_QUEUE
#define MST_PRIO_QUEUE

#include <cstdlib>
#include <cstdio>

namespace base
{
    /**
     * \brief virtual class for having more than one priority-queues with a generic interface (p should be of pointer-type)
     * \author Stefan Naumann
     * \date 27.04.2014
     **/
    template <typename t, typename p>
    class _prio_queue
    {   
        public:
            /**
             * \brief a function for adding a key with pointer to values.
             * \param[in] k priority of the new element
             * \param[in] ptr pointer to the value-object
             * \author Stefan Naumann
             * \date 27.04.2014
             **/
            virtual void add(t k, p ptr)=0;
            /** 
             * \brief a function for getting the element with the minimal priority
             * \param[out] k the key of the found element
             * \return p pointer to the element holding the date
             * \author Stefan Naumann
             * \date 27.04.2014
             **/
            virtual p minimum(t* k)=0;
            /** 
             * \brief a function for getting the element with the maximal priority
             * \param[out] k the key of the found element
             * \return p pointer to the element holding the date
             * \author Stefan Naumann
             * \date 27.04.2014
             **/
            virtual p maximum(t* k)=0;
            /** 
             * \brief extracts the Element with exactly that key-value
             * \param[in] k the key of the element to be extracted
             * \return p pointer to the element holding the date
             * \author Stefan Naumann
             * \date 27.04.2014
             **/
            virtual p extract(t k)=0;
            /** 
             * \brief removes the element with exactly that key-value
             * \param[out] k the key of the element to be removed
             * \author Stefan Naumann
             * \date 27.04.2014
             **/
            virtual void remove(t k)=0;
    };
}

#endif
