/****
 * Developed by Stefan Naumann, 2013
 * email: snaumann2912@yahoo.de
**/

#ifndef BASE_PARSER_XML
#define BASE_PARSER_XML

#include "string/cstring.h"

namespace base { namespace parser {

class dom;
class dom_node;
class dom_node_attr;

/**
 * \brief xml-class for doing some basic xml-dom stuff
 */
class xml 
{
    private:
        /// DOM-tree pointer
        dom* tree;
        
    public:
        /// type of the node (in use?)
        enum nodetype {
            TEXT, ///< Textnode
            COMMENT, ///< Commentnode
            INSTRUCTION, ///< Instructionnode
            NORMAL, ///< Normalnode
            NOTUSEFUL ///< NON-useful node
        };
    
        /**
         * \brief constructor. 
         * \param mydom pointer to the actual dom 
         */
        xml ( dom* mydom ) : tree (mydom) {};
        
        /**
         * \brief search the name (node) in dom.
         * \param name the name you want to search in dom-tree
         * \return the first node
         */
        dom_node* search ( const char* name );
        /**
         * \brief get the attr_node from the element with specified attribute name
         * \param elem dom_node* element for finding the attribute
         * \param name the name of the element
         * \return dome_node_attr* element if found. If not: NULL
         */
        dom_node_attr* getAttrNode ( dom_node* elem, const char* name );
        
        /**
         * \brief returns true if the attribute node with the specified name exists.
         * \param elem the element the attribute is attached to
         * \param name the name of the attribute node to be searched for
         * \return true if exists, false if not.
         */
        bool hasAttr ( dom_node* elem, const char* name );
        /**
         * \brief returns true if the attribute exists and the value is correct.
         * \param elem the element the attribute is attached to
         * \param name the name of the attribute node to be searched for
         * \param val value of the node to be checked.
         * \return true if attribute node exists and the value is correct.
         */
        bool isAttr ( dom_node* elem, const char* name, const char* val );
        
        /**
         * \brief get the next (following or descendant) element with the name
         * \param elem the start element
         * \param name of the element to be found
         * \return dom_node* element found
         */
        dom_node* nextSibling ( dom_node* elem, const char* name );
        /**
         * \brief get the prev (preceding or descendant) element with the name
         * \param elem the start element
         * \param name of the element to be found
         * \return dom_node* element found
         */
        dom_node* prevSibling ( dom_node* elem, const char* name );
        
        /**
         * \brief find the next child element with the according name
         * \param elem the element to start
         * \param name the name of the element to be found
         * \return dom_node* element found
         */
        dom_node* child ( dom_node* elem, const char* name );
        
        /**
         * \brief use xpath syntax to find an element
         * \param path string to be interpreted as xpath
         * \return dom_node* element found
         */
        dom_node* xpath ( const char* path );       
        
        /**
         * \brief parses an xml-file 
         * \param filename the filename of the xml files to be processed
         * \return xml-object to be rendered from parsing the xml file
         */
        static xml* parsexml( const char* filename );
        
        /**
         * \brief generates the xml file text for exporting to file
         * \return returns the text (pointer to char).
         */
        char* text();
        /** 
         * \brief write the xml-file back from dom-tree
         * \param filename the file to be written
         */
        void write( const char* filename );
};

/**
 * \brief dom tree container, hold the root node
 */
class dom 
{
    private:
        /// pointer to root node of the tree. There is only ONE root-node in xml file
        dom_node* root;
        
    public:
        /**
         * \brief constructor. Sets the root node
         * \param rt the to be root node
         */
        dom ( dom_node* rt = NULL ) : root (rt) {};
        
        /**
         * \brief changes the root node from dom tree
         * \param rt the new root node
         */
        void changeRoot ( dom_node* rt );
    
        /**
         * \brief searches for *name* in dom-tree
         * \param name the name of the element to be searched for
         * \return dom_node* first node to be found
         */
        dom_node* search ( const char* name );
        /**
         * \brief returns the dom_node_attr* found under element elem
         * \param elem the element attached to the to be found attr-list
         * \param name the name of the attribute to be found
         * \return dom_node_attr* found
         */
        dom_node_attr* getAttrNode ( dom_node* elem, const char* name );
        
        /**
         * \brief returns true if the attribute with the name *name* exists
         * \param elem the element attached to the attr-list
         * \param name the name of the attribute node
         * \return true if the attribute node exists, false if not.
         */
        bool hasAttr ( dom_node* elem, const char* name );
        /**
         * \brief returns true if the attribute exists and the value is correct.
         * \param elem the element attached to the attr-list
         * \param name the name of the attribute
         * \param val the value to be checked for
         * \return true if the value is correct, false if attribute does not exist or the value is incorrect
         */
        bool isAttr ( dom_node* elem, const char* name, const char* val );
        
        /**
         * \brief searches in following-axis for element
         * \param elem startelement
         * \param name the name to be looked for
         * \return dom_node* the found node
         */
        dom_node* nextSibling ( dom_node* elem, const char* name );
        /**
         * \brief searches in preceding-axis for element
         * \param elem startelement
         * \param name the name to be looked for
         * \return dom_node* the found node
         */
        dom_node* prevSibling ( dom_node* elem, const char* name );
        
        /**
         * \brief searches in child-axis for element
         * \param elem startelement
         * \param name the name to be looked for
         * \return dom_node* the found node*
         */
        dom_node* child ( dom_node* elem, const char* name );
               
        /**
         * \brief use xpath to find a dom_node
         * \param path the xpath-expression to find the element
         * \return dom_node* the wanted element
         */ 
        dom_node* xpath ( const char* path );      
};

class dom_node
{
    private:
        enum xml::nodetype type;
        
        char* name;
    
        dom_node* next;
        dom_node* prev;
        
        dom_node* child;
        dom_node* parent;
        
        dom_node_attr* attr;

        
    public:
        dom_node ( const char* nm, enum xml::nodetype tp, dom_node* prnt=NULL ):
            type(tp), parent(prnt)
        {
            cstring::strncpy ( nm, name, cstring::strlen(nm) );
                   
            attr=NULL;
            child=NULL;
            next=NULL;
            prev=NULL;
        }
        
        void addChild ( dom_node* obj );
        void addSibling ( dom_node* obj );
        void addAttr ( const char* name, const char* value );
        void addAttrNode ( dom_node_attr* obj );
        
        // INTERN: searches in right! siblings and children of this node
        // recursive! May be extremely slow
        dom_node* search ( const char* name );
        dom_node_attr* getAttrNode ( const char* name );
        char* getAttrValue ( const char* name );
        
        bool hasAttr ( const char* name );
        bool isAttr ( const char* name, const char* val );
        
        dom_node* nextSibling ( const char* name );
        dom_node* prevSibling ( const char* name );
        dom_node* nextSibling ();
        dom_node* prevSibling ();
        
        dom_node* getChild ( const char* name );
        
        static dom_node* parsexml ( const char* str, int length, dom* prnt );
        
        dom_node* xpath ( const char* path );      
};

class dom_node_attr
{
    friend class dom_node;

    private:
        dom_node_attr* next;
        dom_node* parent;
        
        char* name;
        char* value;
        
    public: 
        dom_node_attr ( const char* nm, const char* val, dom_node* prnt) :
            parent (prnt)
        {
            cstring::strncpy( nm, name, cstring::strlen(nm) );
            cstring::strncpy( val, value, cstring::strlen(val) ); 
            next=NULL;
        }
    
        char* getValue();
        char* getAttrValue ( const char* name );
        dom_node_attr* getAttrNode( const char* name );
        
        static dom_node_attr* parsexml ( const char* str, int length, dom_node* prnt );
};

}}

#endif
