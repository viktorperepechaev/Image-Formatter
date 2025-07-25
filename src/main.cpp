#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "headers/cxxopts.hpp"
#include "headers/image.hpp"

int main(int argc, char** argv) {
  // TODO: Сделать проверку аргументов сразу

  cxxopts::Options options(
      "ImageFormatter",
      "This program formats the input picture based on cli arguments");

  options.add_options()("i,input", "Input file", cxxopts::value<std::string>())(
      "o,output", "Output file", cxxopts::value<std::string>())(
      "operations", "Names of operations to perform",
      cxxopts::value<std::vector<std::string>>());

  options.parse_positional({"operations"});

  auto result = options.parse(argc, argv);

  if (!result.contains("input") || !result.contains("output")) {
    throw std::runtime_error(
        "Input or Output files are not mentioned in the argument list");
  }

  Image image(result["input"].as<std::string>());

  std::vector<std::pair<std::string, std::vector<std::string>>> operations;

  const auto& argument_list = result.arguments();

  for (size_t operation_index = 0; operation_index < argument_list.size();
       ++operation_index) {
    if (argument_list[operation_index].key() != "operations") {
      continue;
    }

    operations.emplace_back(
        argument_list[operation_index].value(),
        [&argument_list, &operation_index]() -> std::vector<std::string> {
          const std::string& val = argument_list[operation_index].value();

          if (val == "darken") {
            const std::string& val1 =
                argument_list[operation_index + 1].value();
            ++operation_index;
            return {val1};
          } else if (val == "invert") {
            return {};
          } else if (val == "ref-hor") {
            return {};
          } else if (val == "reh-ver") {
            return {};
          } else if (val == "rotate") {
            const std::string& val1 =
                argument_list[operation_index + 1].value();
            ++operation_index;
            return {val1};
          } else {
            throw std::runtime_error("Unknown argument: " + val);
          }
        }());
  }

  std::unordered_map<std::string,
                     std::function<void(Image&, std::vector<std::string>)>>
      call_table;

  call_table["darken"] =
      [](Image& local_image,
         const std::vector<std::string>& operation_arguments) {
        local_image.Darken(std::stoul(operation_arguments[0]));
      };
  call_table["invert"] =
      [](Image& local_image,
         const std::vector<std::string>& operation_arguments) {
        local_image.Invert();
      };
  call_table["ref-hor"] =
      [](Image& local_image,
         const std::vector<std::string>& operation_arguments) {
        local_image.ReflectHorizontally();
      };
  call_table["ref-ver"] =
      [](Image& local_image,
         const std::vector<std::string>& operation_arguments) {
        local_image.ReflectVertically();
      };
  call_table["rotate"] =
      [](Image& local_image,
         const std::vector<std::string>& operation_arguments) {
        local_image.Rotate(std::stoi(operation_arguments[0]));
      };

  for (const auto& operation : operations) {
    call_table[operation.first](image, operation.second);
  }

  image.CreateOutputImage(result["output"].as<std::string>());

  return 0;
}

