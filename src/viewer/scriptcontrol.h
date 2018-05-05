#include <engine/engine.h>
#include <thread>
#include <mutex>
#include <vector>

class ScriptControl
{
    std::thread slave;
    std::mutex jobMutex;
    std::vector<std::string> inputScripts;
    std::vector<std::shared_ptr<Generator>>   outputResults;

    size_t iterationCount;
    float stochasticity;
    bool shouldTerminate;
    bool _hasJob;

    // path to last script used
    std::string lastScriptName;
    public:
    ScriptControl()
    {
        this->shouldTerminate = false;
        this->_hasJob = false;
    }
    ~ScriptControl()
    {
        jobMutex.lock();
        this->shouldTerminate = true;
        jobMutex.unlock();
        this->slave.join();
    }
    void addScript(std::string name, size_t iterationCount = 1, float stochasticity = 0.5)
    {
        this->jobMutex.lock();
        this->inputScripts.push_back(name);
        this->lastScriptName = name;
        this->iterationCount = iterationCount;
        this->stochasticity = stochasticity;
        this->jobMutex.unlock();
    }
    const std::string& getLastScriptName() const { return this->lastScriptName; }
    void init()
    {
        slave = std::thread([this]() {
            while(1==1)
            {
                std::string scriptName = "";
                jobMutex.lock();
                if(this->shouldTerminate)
                {
                    this->jobMutex.unlock();
                    return;
                }
                this->_hasJob = false;
                if(this->inputScripts.size() > 0)
                {
                    scriptName = this->inputScripts.back();    
                    this->inputScripts.pop_back();
                    this->_hasJob = true;
                }
                jobMutex.unlock();
                if(scriptName == "")
                {
                      std::this_thread::sleep_for(std::chrono::milliseconds(10));
                      continue;
                } else {
                    LOG_INFO("Doing something with %s\n",scriptName.c_str());

                    auto gen = std::make_shared<Generator>();
                    if(gen->compile(scriptName.c_str()))
                    {
                        // set iteration uniform
                        gen->getLibrary().setUniform("iterations", (int) this->iterationCount);
                        gen->getLibrary().setUniform("stochasticity", (float) this->stochasticity);
                        bool generationResult = false;
                        if(gen->generate() != false)
                        {
                            if(scriptName.find("cube") != std::string::npos)
                            {
                                this->jobMutex.lock();
                                this->outputResults.push_back(gen);
                                this->jobMutex.unlock();
                            } 
                        }
                        if(generationResult == false)
                            LOG_ERROR("Generation failed\n");
                    } else {
                        LOG_ERROR("Procedural compilation failed\n");
                    }


                }
            }  
        });
    }

    void tick(std::shared_ptr<SceneNode> scene) {
        std::lock_guard<std::mutex> lock(jobMutex);
        if(outputResults.size() == 0)
        {
            return;
        } else {
            auto newNode = std::make_shared<SceneNode>();
            auto result = this->outputResults.back()->produceOutput(newNode);

            if(result != 0)
            {
                scene->clearChildren();
                scene->addChild(newNode);
            }
            this->outputResults.pop_back();
        }
   }

   bool hasJob()
   {
       return this->_hasJob;
   }
};
