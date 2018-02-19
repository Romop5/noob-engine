#include <engine/engine.h>
#include "procgen.h"
class Generator
{

    ProcGen::Generation pg;
    public:
        bool compile(std::string filePath)
        {
            //pg.setDebugOn(false);

            if(this->pg.parseFile(filePath) == false)
                return false;
            if(this->pg.isReady())
            {
                return true;
            }
            return false;

        }
        bool generate(std::shared_ptr<SceneNode> parent)
        {
             if(this->pg.runInit() == false)
                    return false;

                this->pg.run(3);
                json result = this->pg.serialize();
                produceGeometryFromJson(parent, result);
                return true; 
        }
        void produceGeometryFromJson(std::shared_ptr<SceneNode> parent, json input)
        {

                LOG_DEBUG("Result of generation: \n%s\n",input.dump(1).c_str());
        }
};
