#pragma once

namespace gall {
    class EvolvingEnvironment {
    public:
        unsigned int populationSize;
        unsigned int genesCount;
        unsigned int parentsPerChild;
        unsigned int numberOfThreads;
        unsigned int targetGenerationsCount;
        EvolvingEnvironment() : populationSize(100), genesCount(100),
                parentsPerChild(2), numberOfThreads(1), targetGenerationsCount(100) {
            // do nothing
        }
    };

    class EvolvingEnvironmentProvider {
    public:
        static EvolvingEnvironment& getInstance() {
            static EvolvingEnvironment instance;
            return instance;
        }

        EvolvingEnvironmentProvider(const EvolvingEnvironmentProvider&) = delete;
        void operator=(const EvolvingEnvironmentProvider&) = delete;
    };
}
