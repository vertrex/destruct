#ifndef DESTRUCT_DSERIALIZE_HPP_
#define DESTRUCT_DSERIALIZE_HPP_

#include <iostream>
#include <vector>

#include "dvalue.hpp"
#include "dobject.hpp"
//class DObject;
//#include "dobject.hpp"

/* XML Serializer */


/*class Serializer //factory avec registrration de type XML, text , binary , raw ... remplace showObjectAttribute par un serializer !!! //show struct en mode int blabla comme on fait avec output ca sera deja bcp plus style :) et mettre les test ds un sous repertoire plutot appellerca exemple que test ce qu on fait ici !
{

}

class SerializeXML : public serialize
*/

//commencons doucement :)
//en faite c la meme class showObject attribute on peu meme change juste le XMLTag vers un texttag lol :)

//pattern visiteur avec handle ou autre ? classique ? 
//-> penser a tous les formats ok xml, binary, text, raw data pour unserialize mais c la facon simple
// couche superieur pour destruct qui utilise le raw
// couche superieur pour archive /save load ( path du fichier ( et choisir ce qu on veux serializer) 
// DFFArchive doit pouvoir utiliser dserialize  mais pas d 'un coup 
// genre il a sont format avec struct {
/*
  destructheader* -> serialize(DStruct, "binary"(
  destructobject processus -> serialize(DObject process, ""bin"
  destructObject module
                for each moduel load/save
                        arguments
                tags
                etc.
                etc...


penser data (remplacement des nodes avec des method filemaping, sequences [], read, seek etc... !) 
car un dobject->data (type qui existe pas encore donc a rajouter) doit pouvoir etre deserializer (pas pour la sauvegarde mais pour le raw) aussi bien qu un fd etc...

//XXX ajouter la serialization / deserialization directement ds DObject ? ou ds un deriver dsimple ddynamic ? 
// permet de faire toto.serialize("xml", std::cout) ou std::cout << toto.serialize("xml")
// ou DStruct toto.newObject()  toto.unserialize("xml", "buff")  toto.unserialize()....
//    toto << >> toto.newObject("xml", buff)

*/

namespace Destruct
{

class DSerialize
{
public:
  virtual const std::string  name(void) = 0;
  virtual bool serialize(std::ostream& output, DObject&  dobject)  = 0;
  virtual bool unserialize(std::ostream& output, DObject& dobject) = 0;
  //
  //virtual boool serialize(std::ostream& output, DStruct& dstruct) = 0; 
  //virtual boool serialize(std::ostream& output, DStruct& dstruct) = 0; 

  virtual DSerialize* create(void) = 0;
};

class DSerializeXML : public DSerialize
{
public :
  DSerializeXML()
  {
  };

  const std::string name(void);
  DSerializeXML*   create(void);
  bool serialize(std::ostream &output, DObject& dobject); 
  bool serialize(std::ostream& output, DObject& dobject, int depth);
  bool unserialize(std::ostream &input, DObject& dobject);
//serialize DStruct / Destruct ? 
private:

  class XMLTag
  {
  public:
    XMLTag(std::ostream& output, std::string const & tag, std::string const & value = "", int depth = 1, bool enclose = false) : __output(output), __tag(tag), __depth(depth), __enclose(enclose)
    {
      output  << std::string(2*(depth - 1), ' ') <<  '<' << tag << '>' << value;   
    }

    ~XMLTag()
    {
     if (this->__enclose)
       this-> __output << std::string(2*(this->__depth - 1), ' ') <<  "</" <<this-> __tag << '>' << std::endl;
     else
       this-> __output <<  "</" <<this-> __tag << '>' << std::endl;
    }

  private:
    std::ostream&      __output;
    const std::string  __tag;
    int                __depth;
    bool               __enclose;
  };
};

class DSerializeText : public DSerialize
{
public :
  DSerializeText()
  {
  };

  const std::string name(void);
  DSerializeText*   create(void);
  bool serialize(std::ostream &output, DObject& dobject); 
  bool serialize(std::ostream& output, DObject& dobject, int depth);
  bool unserialize(std::ostream &input, DObject& dobject);
};

//initialization ds session api.init ?
class DSerializers
{
public:
  DSerializers();
  static DSerialize* to(const std::string type);
  static DSerialize* to(size_t id);
  static size_t      count(void);
  static bool        registerSerializer(DSerialize* serializer);
private:
  static std::vector<DSerialize*  > __serializers;
};

}

#endif
