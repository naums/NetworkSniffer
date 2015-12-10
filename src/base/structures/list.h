/**
 * \file simple linked-list datastructure
 * \author Stefan Naumann (me@stefannaumann.de)
 * \date 2013
**/

#ifndef BASE_STRUCTURES_LIST
#define BASE_STRUCTURES_LIST

#include <cstdlib>

namespace base { 

    template <typename t> class list_node;

    /**
     * \brief class for list; beware, that putting a char* in there could 
     * result in unusable search-functions 
     */
    template <typename t>
    class list
    {
        /// list_node needs to be friend for reading first / last
        friend class list_node<t>;
    
        private:
            /// first element of the list
            list_node<t>* first;
            /// last element of the list
            list_node<t>* last;
            
        public:
            /**
             * \brief constructor. Sets first and last to NULL.
             */
            list()
            {
                this->first=NULL;
                this->last=NULL;
            }
            
            /**
             * \brief destructor. Deletes all elements of the list.
             */
            ~list()
            {
                if (this->first!=NULL)
                    delete this->first;
            }
            
            /**
             * \brief deletes all the values of all nodes, 
             * should only be used on malloc'd values
             */
            void clean ()
            {
                list_node<t>* tmp=this->first;
                while (tmp)
                {
                    tmp->clean();
                    tmp=tmp->next;
                }
            }
            
            int getSize()
            {
                list_node<t>* tmp=this->first;
                int i=0;
                while (tmp)
                {
                    i++;
                    tmp=tmp->next;
                }
                return i;
            }
            
            /**
             * \brief returns the first element (pointer to first element) of the list
             * \return pointer to first element (list_node<t>)
             */
            list_node<t>* getFirst()
            {
                return this->first;
            }
            /**
             * \brief returns pointer to last element
             * \return pointer to last element (list_node<t>)
             */
            list_node<t>* getLast()
            {
                return this->last;
            }
            
            /**
             * \brief searches for the element with value=key. With char* as type 
             * may become unusable
             * \param key the value to be searched for
             * \return pointer to the list_node<t>-element. 
             */
            list_node<t>* search( t key )
            {
                list_node<t>* tmp=this->first;
                if (tmp==NULL) 
                    return NULL;
                if (tmp->value==key)
                    return tmp;
                    
                while (tmp->next!=NULL)
                {
                    tmp=tmp->next;
                    if (tmp->value==key)
                        return tmp;
                }
                
                return NULL;
            }
            
            /**
             * \brief removes the referenced node from a list.
             * \param node pointer to the node object to be removed.
             * \return nothing.
             */
            void remove ( list_node<t>* node )
            {
                list_node<t>* tmp=node->prev;
                if (tmp!=NULL)
                    tmp->next=node->next;
                tmp=node->next;
                if (tmp!=NULL)
                    tmp->prev=node->prev;
                node->prev=NULL;
                node->next=NULL;
                delete node;
            }
            /**
             * \brief adds the referenced node object to the list.
             * \param node pointer to the node object to be added.
             * \return nothing
             */
            void add ( list_node<t>* node )
            {
                if (this->last==NULL && this->first==NULL)
                {
                    this->last=node;
                    this->first=node;
                }
                else
                {
                    list_node<t>* tmp=this->last;
                    tmp->next=node;
                    node->prev=tmp;
                    this->last=node;                    
                }
            }
            
            /**
             * \brief creates a new node with a specific value and adds it to the list
             * \param value the value for saving in that node
             * \return nothing
             */
            void add ( t value )
            {
                list_node<t>* node=new list_node<t>(value);
                this->add(node);
            }
            
            bool exists ( t value )
            {
                return (this->search ( value ) != NULL);
            }
    };
    
    /**
     * \brief class for list_node.
     */
    template <typename t>
    class list_node
    {    
        /// makes list-class friend of list_node
        friend class list<t>;
    
        private:
            /// saved value of the list_node object.
            t value;      
                
        public:
            /// pointer to the next element in list (or NULL if it doesn't exist)
            list_node<t>* next;
            /// pointer to the previous element in list (or NULL if it doesn't exist)
            list_node<t>* prev; 
         
            /**
             * \brief destructor. Deletes this element and the next one (and its next one,...)
             * may result to memory leaks if the value is a pointer to malloc'd space.
             */
            ~list_node()
            {
                if (this->next!=NULL)
                    delete this->next;
            }
         
            /**
             * \brief constructor. will set the value and will initialize next and prev pointers.
             * \param valu the value to be set for this node-element
             */
            list_node(t valu) 
                : value (valu) 
            {
                this->next=NULL;
                this->prev=NULL;
            }
            
            /**
             * \brief deletes the value, should only be used on malloc'd values
             */
            void clean ()
            {
                delete this->value;
                this->value=NULL;
            }
            
            /**
             * \brief will return the value (type t) of this node-element
             * \return this->value;
             */
            t getValue()
            {
                return this->value;
            }   
            
            /**
             * \brief sets a new value of this node-element. May result to memory leaks
             * if the value was a pointer to malloc'd space.             
             * \param val the new value to be set
             * \return nothing
             */             
            void setValue ( t val ) 
            {
                this->value=val;
            }
    };

}

#endif
