
#ifndef IOC_CONTAINER_HPP
#define IOC_CONTAINER_HPP

#include <memory>
#include <map>
#include <functional>
#include <typeindex>
#include <tuple>
#include <vector>

/*
 * @Brief: Exception used by IocContainer when requesting unregistered services.
 */
class ServiceNotAvailable : public std::exception
{
    public:
        const char * what() const throw ()
        {
            return "ServiceNotAvailable";
        }
};

/*
 * @Brief: Exception used by IocContainer when requesting a service that has circular dependencies.
 */
class CircularDependency : public std::exception
{
    public:
        const char * what() const throw ()
        {
            return "CircularDependency";
        }
};

/*
 * @Brief: Base class that services must derive from.
 */
class ServiceBase
{
    public:
        virtual ~ServiceBase(){};
};

/*
 * @Brief: IocContainer class, it allows to register Services and instantiate objects of them.
 *
 *        Limitations:
 *            - It can only instantiate transient objects
 *            - Registered services cannot have more than 1 dependency
 *            - Registered services cannot have any input parameter in the constructor
 */
class IocContainer
{
    /*
     * @Brief: Factory method type definition, used to create services passing the dependency as a parameter
     */
    using ServiceFactory = std::function<std::shared_ptr<ServiceBase>(std::shared_ptr<ServiceBase>)>;

    public:

        /*
         * @Brief: Method to retrieve the IocContainer singleton
         *
         * [return value] singleton object
         */
        static IocContainer& GetSingleton();

        /*
         * @Brief: Method to register services with no depencencies.
         *
         * [templated type] ServiceInterface      : Service interface that wants to be registered, it must derive from ServiceBase
         * [templated type] ServiceImplementation : Service implemenation class, it must derive from ServiceInterface
         */
        template<typename ServiceInterface, typename ServiceImplementation>
        void RegisterService()
        {
            static_assert(std::is_base_of<ServiceInterface, ServiceImplementation>::value, "ServiceInterface needs to be parent of ServiceImplementation");

            auto service_type_id = std::type_index(typeid(ServiceInterface));
            auto nullptr_type_id = std::type_index(typeid(std::nullptr_t)); /* this is only used to indicate that the service does not have dependencies */

            auto factory = [](std::shared_ptr<ServiceBase> null_dependency){ return std::make_shared<ServiceImplementation>();};

            /* Store in the factory_map: service type id, its factory method */
            factory_map.emplace(service_type_id, std::make_pair(factory, nullptr_type_id));
        }

        /*
         * @Brief: Method to register services with no depencencies.
         *
         * [templated type] ServiceInterface      : Service interface that wants to be registered, it must derive from ServiceBase
         * [templated type] ServiceImplementation : Service implemenation class, it must derive from ServiceInterface
         * [templated type] ServiceDependency     : Dependency required by the service to be built
         */
        template<typename ServiceInterface, typename ServiceImplementation, typename ServiceDependency>
        void RegisterService()
        {
            static_assert(std::is_base_of<ServiceInterface, ServiceImplementation>::value, "ServiceInterface needs to be parent of ServiceImplementation");

            auto service_type_id = std::type_index(typeid(ServiceInterface));
            auto dependency_type_id = std::type_index(typeid(ServiceDependency));

            auto factory = [](std::shared_ptr<ServiceBase> dependency){ return std::make_shared<ServiceImplementation>(std::dynamic_pointer_cast<ServiceDependency>(dependency));};

            /* Store in the factory_map: service type id, its factory method and dependency */
            factory_map.emplace(service_type_id, std::make_pair(factory, dependency_type_id));
        }

        /*
         * @Brief: Method to instantiate registered services objects.
         *
         *        ServiceNotAvailable exception is thrown if the service is not registered
         *        CircularDependency exception is thrown if a circular dependency is found while instantiating the service
         *
         * [templated type] ServiceInterface : Service interface from which we want to instantiate an object
         *
         * [return value] : pointer to the created service object
         */
        template<typename ServiceInterface>
        std::shared_ptr<ServiceInterface> GetService()
        {
            return std::dynamic_pointer_cast<ServiceInterface>(ResolveService(std::type_index(typeid(ServiceInterface)),{}));
        }

        /*
         * @Brief: Method to remove services
         *
         * [templated type] ServiceInterface : Service interface that wants to be removed
         */
        template<typename ServiceInterface>
        void RemoveService()
        {
            factory_map.erase(std::type_index(typeid(ServiceInterface)));
        }

    private:
        /*
         * @Brief: Private container to prevent clients from creating object of this class
         */
        IocContainer(){};

        /*
         * @Brief: Factory map (an associative array) for storing the service types and its factories and dependencies
         *
         *         [key] : type_index of the service class
         *         [Value] : a pair of,
         *                       -ServiceFactory method to create the service
         *                       -type_index of the service's dependency class (std::nullptr_t if the service has no dependencies)
         */
        std::map<std::type_index, std::pair<ServiceFactory, std::type_index>> factory_map;

        /*
         * @Brief: Function to instantiate services. It uses the factory map to recursively creating the chain
         *         of dependencies and the final requested service object
         *
         * [in] service_id       : class id of the service to be resolved
         * [in] service_id_chain : chain of resolved dependencies, this list helps to protect from circular dependencies
         */
        std::shared_ptr<ServiceBase> ResolveService(std::type_index service_id, std::vector<std::type_index> service_id_chain)
        {
            /* If the service to be resolved is not present in the factory map, throw an exception */
            if(factory_map.find(service_id) == factory_map.end())
            {
                throw ServiceNotAvailable();
            }

            /* Circular dependency protection, if the service has been already resolved, throw an exception */
            if(std::find(service_id_chain.begin(), service_id_chain.end(), service_id) != service_id_chain.end())
            {
                throw CircularDependency();
            }

            /* Include the current service in the dependency chain */
            service_id_chain.push_back(service_id);

            auto factory            = factory_map.at(service_id).first;
            auto dependency_type_id = factory_map.at(service_id).second;

            /* Stops the recursion: the service doesn't have a dependency the factory can create the object immediately */
            if (dependency_type_id == std::type_index(typeid(std::nullptr_t)))
            {
                return factory({});
            }
            /* Recursion: the service has a depencency, the dependency needs to be created before calling the factory */
            else
            {
                return factory(ResolveService(dependency_type_id, service_id_chain));
            }
        }
};

#endif