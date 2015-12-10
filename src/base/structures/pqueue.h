/** \file pqueue.h
 * \brief provides a priority-queue data-structure
 * \author Stefan Naumann
 * \date 2014
 **/

#ifndef MST_OPENCL_PQUEUE
#define MST_OPENCL_PQUEUE

#include "prioqueue.h"

namespace base {

    template <typename t, typename p> class pqueue_node;

    /**
     * \brief class for priority-queue. adding does insertion sort, extracting works in O(1)
     *        t is the type of the key (eg. float, int) for sorting, p is the type of the payload (eg. pointer to a struct)
     * \author Stefan Naumann
     **/
    template <typename t, typename p>
    class pqueue : public _prio_queue<t,p>
    {
        private:
            /// first pointer, points to the first (minimal) element of the queue
            pqueue_node<t,p>* first;
            /// points to the last (maximal) element of the queue
            pqueue_node<t,p>* last;
            /// sum of all element-keys
            t sum;
            /// number of elements in the queue
            int number;
            
        public:
            /**
             * \brief constructor, sets first and last pointers
             * \author Stefan Naumann
             **/
            pqueue()
            {
                this->first=NULL;
                this->last=NULL;
                this->sum=0;
                number=0;
            }
            
            /**
             * \brief destructor, deletes every node element of the queue
             * \warn does not delete the payload pointed to from within the nodes!
             * \author Stefan Naumann
             **/
            virtual ~pqueue()
            {
                delete this->first;
            }
            
            /**
             * \brief adds a new element
             * \param[in] key the key for sorting
             * \param[in] ptr the payload (can be a pointer to struct)
             * \author Stefan Naumann
             **/
            void add(t key, p ptr)
            {
                pqueue_node<t,p>* tmp=new pqueue_node<t,p>(key, ptr);
                if (this->first==NULL)
                {
                    this->first=tmp;
                    this->last=tmp;
                
                    number++;
                    this->sum=key;
                }
                else
                {
                    t avg = 0;
                    if (number>0)
                        avg = sum / number;
                    pqueue_node<t,p>* current=NULL;

                    if (avg >= tmp->key)
                    {
                        current=this->first;
                        while (current!=NULL)
                        {
                            if (tmp->key <= current->key)
                            {
                                if (current!=first && current!=last)
                                {
                                    tmp->next=current;
                                    tmp->prev=current->prev;
                                    current->prev=tmp;
                                    current=tmp->prev;
                                    current->next=tmp;
                                }
                                else if (current==first)
                                {
                                    first=tmp;
                                    tmp->next=current;
                                    current->prev=tmp;
                                }
                                /*else if (current==first && current==last)
                                {
                                    last=tmp;
                                    tmp->prev=current;
                                    current->next=tmp;
                                }*/
                                else
                                {
                                    tmp->next=current;
                                    tmp->prev=current->prev;
                                    current->prev = tmp;
                                    current=tmp->prev;
                                    current->next=tmp;
                                }
                                                                
                                number++;
                                sum+=key;
                                return;
                            }
                        
                            current=current->next;
                        }
                        
                        current=this->last;
                        last=tmp;
                        tmp->prev=current;
                        current->next=tmp;
                        
                        number++;
                        sum+=key;
                    }
                    else
                    {
                        current=this->last;
                        while (current!=NULL)
                        {
                            if (tmp->key >= current->key)
                            {
                                if (current!=first && current!=last)
                                {
                                    tmp->prev=current;
                                    tmp->next=current->next;
                                    current->next=tmp;
                                    current=tmp->next;
                                    current->prev=tmp;
                                }
                                /*else if (current==first && current!=last)
                                {
                                    first=tmp;
                                    tmp->next=current;
                                    current->prev=tmp;
                                }*/
                                else if (current==last)
                                {
                                    last=tmp;
                                    tmp->prev=current;
                                    current->next=tmp;
                                }
                                else
                                {
                                    tmp->prev=current;
                                    tmp->next=current->next;
                                    current->next=tmp;
                                    current=tmp->next;
                                    current->prev=tmp;
                                }
                                                                
                                number++;
                                sum+=key;
                                return;
                            }
                        
                            current=current->prev;
                        }
                        
                        current=this->first;
                        first=tmp;
                        tmp->next=current;
                        current->prev=tmp;
                        
                        number++;
                        sum+=key;
                    }
                }
            }
            
            /**
             * \brief returns the payload of the minimum-node and deletes the node!
             * \param[out] k the key of the element, if wanted
             * \return the payload of the first (minimal) node or NULL if no element was found
             * \author Stefan Naumann
             **/
            p min_extract ( t* k=NULL )
            {
                pqueue_node<t,p>* tmp=this->first;
                if (tmp==NULL)
                    return NULL;
                    
                if (k!=NULL)
                    *k=tmp->key;    
                    
                this->first=tmp->next;
                tmp->next=NULL;
                p ptr=tmp->pointer;
                
                number--;
                sum-=tmp->key;
                
                delete tmp;
                
                return ptr;
            }
            
            /**
             * \brief returns the payload of the maximum-node and deletes the maximum node!
             * \param[out] k the key of the element, if wanted
             * \return the payload of the last (maximal) node or NULL if no element was found
             * \warn may take O(n) (because of the removing)
             * \author Stefan Naumann
             **/
            p max_extract ( t* k=NULL)
            {
                pqueue_node<t,p>* tmp=this->last;
                if (tmp==NULL)
                    return NULL;
                    
                if (k!=NULL)
                    *k=tmp->key;
                
                this->last=tmp->prev;
                tmp->prev=NULL;
                p ptr=tmp->pointer;
                
                number--;
                sum-=tmp->key;
               
                delete tmp;
                    
                return ptr;
            }
            
            /**
             * \brief returns the payload of the minimum-node. Does NOT delete the node
             * \param[out] k the key of the element, if wanted
             * \return the payload of the first (minimal) node or NULL if no element was found
             * \author Stefan Naumann
             **/
            p minimum(t* k=NULL)
            {
                pqueue_node<t,p>* tmp=this->first;
                if (tmp==NULL)
                    return NULL;
                    
                if (k!=NULL)
                    *k=tmp->key;
                return tmp->pointer;
            }
                     
            /**
             * \brief returns the payload of the minimum-node. Does NOT delete the node.
             * \param[out] k the key of the element, if wanted
             * \return the payload of the first (minimal) node or NULL if no element was found
             * \author Stefan Naumann
             **/   
            p maximum(t* k=NULL)
            {
                pqueue_node<t,p>* tmp=this->last;
                if (tmp==NULL)
                    return NULL;
                    
                if (k!=NULL)
                    *k=tmp->key;
                return tmp->pointer;
            }
            
            /**
             * \brief returns the payload of a specific node or NULL if the node was not found
             * \param[in] k the key of the node to be looked for
             * \return the payload of the node, if found; if not: NULL
             * \author Stefan Naumann
             **/
            p extract(t k)
            {
                pqueue_node<t,p>* tmp=this->first;
                while (tmp!=NULL)
                {
                    if (tmp->key==k)
                        return tmp->pointer;
                    tmp=tmp->next;
                }
                return NULL;
            }
            
            /**
             * \brief cleans the pqeue (wipes payload-data)
             * \author Stefan Naumann
             **/
            void clean()
            {
                if (this->first!=NULL)
                    this->first->clean();
                    
                number=0;
                sum=0;
            }
            
            p index ( int k )
            {
                pqueue_node<t,p>* tmp=this->first;
                for (int i=0; tmp!=NULL && i<=k; i++)
                {
                      if (i==k)
                        return tmp->pointer;
                    tmp=tmp->next;
                }
                
                return NULL;
            }
            
            int getSize () 
            {
                return this->number;
            }
            
            /**
             * \brief removes a specific element (only 1 element per call) from the queue
             * \param[in] k the key of the node to be removed
             * \author Stefan Naumann
             */
            void remove(t k)
            {       
                if (number <= 0)
                    return;
                    
                t avg = sum / number;
                pqueue_node<t,p>* tmp=NULL;
                if ( avg >= k )
                    tmp=this->first;
                else
                    tmp=this->last;
                    
                pqueue_node<t,p>* tmp2=NULL;
                while (tmp!=NULL)
                {
                    if (tmp->key==k)
                    {
                        if (tmp==this->first && tmp!=this->last)
                        {
                            this->first=tmp->next;
                            tmp2=tmp->next;
                            tmp->next=NULL;
                            tmp2->prev=NULL;
                        }
                        else if (tmp==this->last && tmp!=this->first)
                        {
                            tmp2=tmp->prev;
                            tmp2->next=NULL;
                            this->last=tmp2;
                        }   
                        else if (tmp==this->first && tmp==this->last)
                        {
                            this->first=NULL;
                            this->last=NULL;
                        }
                        else
                        {
                            tmp2=tmp->prev;
                            tmp2->next=tmp->next;
                            tmp2=tmp->prev;
                            tmp2->prev=tmp->prev;
                            tmp->next=NULL;
                        }
                        number--;
                        sum-=tmp->key;
                        
                        delete tmp;
                        
                        return;
                    }

                    if ( avg >= k )
                        tmp=tmp->next;
                     else
                        tmp=tmp->prev;
                }
            }   
            
            void debug ()
            {
                pqueue_node<t, p>* tmp=this->first;
                
                printf ("FIRST: %ld, LAST: %ld; NUM: %d, SUM: %f\n", first, last, number, sum);
                
                while (tmp!=NULL)
                {
                    printf ("KEY: %f, PAYL: %d, PREV: %ld, NEXT: %ld\n", tmp->key, tmp->pointer, tmp->prev, tmp->next);
                
                    tmp=tmp->next;
                    
                }
            }
    };
    
    /**
     * \brief class describing the nodes of pqueue. Holds data (or pointer to data)
     * \author Stefan Naumann
     **/
    template <typename t, typename p>
    class pqueue_node
    {
        friend class pqueue<t,p>;
        private:
            /// pointer to the next element within the queue
            pqueue_node<t,p>* next;
            /// pointer to the previous element within the queue
            pqueue_node<t,p>* prev;
            /// the key attribute for sorting
            t key;
            /// payload or pointer to payload
            p pointer;  
            
        public:
            /**
             * \brief constructs the node, fills it with data
             * \param[in] k the key for sorting
             * \param[in] ptr the payload or pointer to payload (see your p-type!)
             * \author Stefan Naumann
             **/
            pqueue_node(t k, p ptr)
            {
                this->key=k;
                this->pointer=ptr;
                this->next=NULL;
                this->prev=NULL;
            }
            
            /**
             * \brief deletes the next element, emptying the queue
             * \author Stefan Naumann
             **/
            ~pqueue_node()
            {
                delete this->next;
            }
            
            /**
             * \brief wipes the payload (MUST be pointer to some ALLOCED! data)
             * \warn do not use this function with non-pointer-payload or pointer to not alloced data!
             * \author Stefan Naumann
             **/
            void clean()
            {
                delete this->pointer;
                if (this->next!=NULL)
                    this->next->clean();
            }
            
            /**
             * \brief returns the pointer of the node
             * \return the pointer of this node
             * \author Stefan Naumann
             **/
            p getPtr()
            {
                return this->ptr;
            }
    };
}

#endif
