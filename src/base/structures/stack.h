/****
 * Developed by Stefan Naumann, 2013 
 * email: snaumann2912@yahoo.de
**/

#ifndef BASE_STRUCT_STACK
#define BASE_STRUCT_STACK

#include <cstdio>
#include <cstdlib>

/// \namespace base-namespace containing all basic elements, classes and methods
namespace base {
    
    /**
     * \enum for describing the wanted behavior of the stack-class
     * \author Stefan Naumann
     * \date 2013
     */
    enum stackType 
    {
        STACK, ///< Sets the stack-object to stack-behavior
        LIFO, ///< Sets the stack-object to stack-behavior
        QUEUE, ///< Sets the stack-object to queue-behavior
        FIFO ///< Sets the stack-object to queue-behavior
    };
    
    template <typename t1> class stack_node;
    
    /**
      * \brief stack / queue class
      *
      * This class is stack and queue in one - you have to set the type accordingly.
      * So if you pass STACK or LIFO to constructor you will get a stack-behavior,
      * if you pass QUEUE or FIFO you will get queue-behavior. 
      * You are able to change the behavior, but it will probably result to not
      * readable code.
      * 
      * \author Stefan Naumann
      * \date 2013
      */
    template <typename t1>
    class stack
    {
        private:
            /// the type of behavior you want from the stack
            enum stackType type;
            /// first element in stack
            stack_node<t1>* first;
            /// last element in stack
            stack_node<t1>* last;
            
        public:
            /**
             * \brief constructor. creates stack / queue and sets the initial behavior.
             * \param tp type of behavior you want from the object
             * \author Stefan Naumann
             * \date 2013
             */
            stack (enum stackType tp):
                type(tp)
            {
                first=NULL;
                last=NULL;
            }
            
            /**
             * \brief destructor. Deletes every element in stack 
             * \author Stefan Naumann
             * \date 2013
             */
            ~stack ()
            {
                stack_node<t1>* tmp=this->first, *tmp2;
                
                while (tmp!=NULL)
                {
                    tmp2=tmp->next;
                    delete tmp;
                    tmp=tmp2;
                }
            }
            
            /**
             * \brief cleans up the stack including values. Be warned: use only with pointer type you malloc'd!
             * \see ~stack()
             * \author Stefan Naumann
             * \date 27.04.2014
             **/
            void clean()
            {
                stack_node<t1>* tmp=this->first, *tmp2;

                while (tmp!=NULL)
                {
                    tmp2=tmp->next;
                    delete tmp->value;
                    delete tmp;
                    tmp=tmp2;
                }
                
                this->first=NULL;
            }
            
            /**
             * \brief pops the last / first element from stack
             *
             * pop returns the last / first element from stack / queue. If you
             * set stack or lifo you will get a stack-like behaviour, 
             * if you set queue or fifo you will get a queue-like behavior.
             * \param <none>
             * \return returns the value of the element in question.
             * \author Stefan Naumann
             * \date 2013
             */
            t1 pop()
            {
                if (type==FIFO || type==QUEUE)
                {
                    if (this->first == this->last && this->first!=NULL)
                    {
                        t1 val=this->first->getValue();
                        this->first=NULL;
                        delete this->last;
                        this->last=NULL;
                        
                        return val;
                    }
                    else if (this->first!=NULL)
                    {
                        t1 val=this->first->getValue();
                        stack_node<t1>* tmp=this->first;
                        if (tmp->next!=NULL)
                        {
                            this->first=this->first->next;
                            this->first->prev=NULL;
                        }
                        else
                        {
                            this->first=NULL;
                            this->last=NULL;
                        }
                        delete tmp;
                        
                        return val;
                    }
                    else
                        return NULL;
                }
                else
                {
                    if (this->first == this->last && this->last!=NULL)
                    {
                        t1 val=this->first->getValue();
                        this->first=NULL;
                        delete this->last;
                        this->last=NULL;
                        
                        return val;
                    }
                    else if (this->last!=NULL)
                    {
                        t1 val=this->last->getValue();                        
                        stack_node<t1>* tmp=this->last;
                        if (tmp->prev!=NULL)
                        {
                            this->last=this->last->prev;
                            this->last->next=NULL;
                        }
                        else
                        {
                            this->first=NULL;
                            this->last=NULL;
                        }
                        delete tmp;
                        
                        return val;
                    }
                    else
                        return NULL;
                }              
            }
            
            /**
             * \brief adds an element by value. (creates the stack_node-object and calls the other push-function 
             * \param val the value to be hold by stack_node-object
             * \return none
             * \author Stefan Naumann
             * \date 2013
             */
            void push ( t1 val ) 
            {
                stack_node<t1>* m = new stack_node<t1> (this, val);
                this->push(m);
            }   
            
            /**
             * \brief pushes the stack_node - object m onto stack
             *
             * the push-operation is not changed by the type of stack / queue. 
             * It will add the element m to the last place everytime.
             *
             * \param m stack_node-object to be added.
             * \return none
             * \author Stefan Naumann
             * \date 2013
             */
            void push ( stack_node<t1>* m )
            {
                if (this->first==NULL && this->last==NULL)
                {
                    this->first=m;
                    this->last=m;
                }
                else
                {
                    this->last = m;
                    stack_node<t1>* n = this->first;
                    
                    while (n->next!=NULL)
                    {
                        n=n->next;
                    }
                    
                    n->next=m;
                    m->prev=n;
                }
            }
            
            /**
             * \brief sets the type of the stack. May result to unreadable code.
             * \param tp type of behavior you want.
             * \return none
             * \author Stefan Naumann
             * \date 2013
             */
            void setType ( enum stackType tp )
            {
                this->type = tp;
            }     
            
            /**
             * \brief returns the type of behavior you do have with your stack.
             * \param <none>
             * \return the current type of behavior (as enum stackType);
             * \author Stefan Naumann
             * \date 2013
             */
            enum stackType getType() 
            {
                return this->type;
            }   
            
            /**
             * \brief returns whether the stack is empty or not.
             * \param <none>
             * \return true if the stack is empty, false if there is at least one object on there
             * \author Stefan Naumann
             * \date 2013
             */
            bool isEmpty()
            {
                if (this->first==NULL)
                    return true;
                return false;
            }      
            
            /**
             * \brief clones the stack and returns the other one (the created stack)
             * \return the new stack
             * \author Stefan Naumann
             * \date 28.09.2014
             **/
            stack<t1>* clone ()
            {
                stack<t1>* nstack = new stack<t1>();
                stack_node<t1>* tmp=this->first;
                stack_node<t1>* tmp2, *tmp3;
                if (tmp!=NULL)
                {
                    tmp2 = new stack_node<t1>( nstack, tmp->value );
                    tmp=tmp->next;
                } 
               
                while (tmp!=NULL)
                {
                    tmp3 = new stack_node<t1>( nstack, tmp->value );
                    tmp2->next= tmp3;
                    
                    tmp2=tmp3;
                
                    tmp=tmp->next;
                }   
                
                return nstack;
            }      
    };
    
    
    
    /**
      * \brief stack_node object. Nodes for the stack / queue
      *
      * the nodes are not per se for a stack; they may be used for the queue as well. 
      * See Stack for more details. 
      * Stack_node may hold any datatype you might want (even pointer to objects!)
      *
      * \author Stefan Naumann
      * \date 2013
      */
    template <typename t1>
    class stack_node
    {
        public:
            /// the next node in stack / queue
            stack_node<t1>* next;
            /// the previous node in stack / queue
            stack_node<t1>* prev;
            /// the parent stack-object of this stack_node object.
            stack<t1>* parent;
            
            /// the value to be hold by stack_node
            t1 value;
            
            /**
             * \brief constructor of stack_node object.
             * \param par the parent object (stack) of the node
             * \param val the value to be set
             * \author Stefan Naumann
             * \date 2013
             */
            stack_node (stack<t1>* par, t1 val = 0): 
                parent (par), value(val) 
            {
                next=NULL;
                prev=NULL;
            }
            
            ~stack_node()
            {
            
            }
            
            /**
             * \brief returns the value of the stack_node object
             * \param <none>
             * \return stack_node value.
             * \author Stefan Naumann
             * \date 2013
             */
            t1 getValue () 
            {
                return this->value;
            }
            /**
             * \brief setValue sets the value of the stack_node.
             * \param val is the value to be set
             * \return none
             * \author Stefan Naumann
             * \date 2013
             */
            void setValue ( t1 val )
            {
                this->value = val;
            }
    };
}

#endif
