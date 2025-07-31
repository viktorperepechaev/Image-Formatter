#pragma once

#include "ImageOperation.hpp"

template <typename SpecificOperation>
class OperationEnforceInterface : public virtual ImageOperation {
 public:
   static bool ValidateArguments(const std::vector<std::string>& arguments) {
     SpecificOperation::ValidateArguments(arguments);
     
     return true;  // Could have written false. This method is never called. Only needed for CRTP.
   }
};
