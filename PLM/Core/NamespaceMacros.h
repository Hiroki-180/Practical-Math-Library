#ifndef CORE_NAMESPACEMACROS_H
#define CORE_NAMESPACEMACROS_H

#pragma once

#define PML_NS_BEGIN namespace pml {
#define PML_NS_END   }
PML_NS_BEGIN PML_NS_END
namespace PMLNS = pml;

#define PML_CONSTANTS_NS_BEGIN PML_NS_BEGIN namespace constants {
#define PML_CONSTANTS_NS_END   } PML_NS_END
PML_CONSTANTS_NS_BEGIN PML_CONSTANTS_NS_END
namespace PMLConstantsNS = pml;

#endif