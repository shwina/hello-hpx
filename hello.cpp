#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>

#include <iostream>
#include <boost/program_options.hpp>

struct hello_component : hpx::components::component_base<hello_component> {

    hello_component() {}

    void hello_function() {
        std::cout << "Hello World\n" << std::endl;
    }
    HPX_DEFINE_COMPONENT_DIRECT_ACTION(hello_component, hello_function, hello_action);
};

typedef hpx::components::component<hello_component> hello_component_type;
HPX_REGISTER_COMPONENT(hello_component_type, hello_component);
HPX_REGISTER_ACTION(hello_component::hello_action);

struct hello_component_client : hpx::components::client_base<hello_component_client, hello_component> {

    typedef hpx::components::client_base<hello_component_client, hello_component> base_type;

    hello_component_client(hpx::future<hpx::id_type> && id)
        :   base_type(std::move(id)){}

    void hello_function() {
        hello_component::hello_action act;
        hpx::async(act, get_id());
    }
};

int hpx_main(boost::program_options::variables_map& vm) {
    std::vector<hpx::id_type> localities = hpx::find_all_localities();
    for (auto locality:localities) {
        hello_component_client c = hpx::new_<hello_component_client>(locality);
        c.hello_function();
    }
    return hpx::finalize();
}

int main(int argc, char *argv[]) {
    // Configure application-specific options.
    boost::program_options::options_description desc_commandline(
        "usage: " HPX_APPLICATION_STRING " [options]");

    return hpx::init(desc_commandline, argc, argv); // Initialize and run HPX.
}
