#include <stdexcept>
#include "main.hh"
#include "entryPoint.hh"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    try {
        auto const& desc = define_options();
        auto const& vm = parse_options(desc, argc, argv);
        if (vm.count("help")) {
            std::cout << desc << std::endl;
        } else {
            return run(vm);
        }
    } catch (po::error &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 2;
    }
    return 0;
}

po::options_description define_options()
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "show usage");
//    desc.add_options()
//        ("image,i", po::value<std::string>(), "input image path");
//    desc.add_options()
//            ("video,v", po::value<std::string>(), "input video path");
//    desc.add_options()
//            ("camera,c", "use default system camera");
//    desc.add_options()
//        ("test,t", "run cpu and gpu implementations then check results equality");
    return desc;
}

po::variables_map parse_options(const po::options_description& desc, int argc,
        char** argv)
{
    po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

    return vm;
}

int run(const po::variables_map& vm)
{
    return run();
}