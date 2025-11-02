/*
 * Copyright (c) 2020 ESWIN Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __EMSIS_VERSION_H
#define __EMSIS_VERSION_H

/**
 * \defgroup EMSIS_Core_VersionControl    Version Control
 * \ingroup  EMSIS_Core
 * \brief    Version \#define symbols for EMSIS release specific C/C++ source code
 * \details
 *
 * We followed the [semantic versioning 2.0.0](https://semver.org/) to control EMSIS version.
 * The version format is **MAJOR.MINOR.PATCH**, increment the:
 * 1. MAJOR version when you make incompatible API changes,
 * 2. MINOR version when you add functionality in a backwards compatible manner, and
 * 3. PATCH version when you make backwards compatible bug fixes.
 *
 * The header file `emsis_version.h` is included by each core header so that these definitions are available.
 *
 * **Example Usage for EMSIS Version Check**:
 * \code
 *   #if defined(__EMSIS_VERSION) && (__EMSIS_VERSION >= 0x00000100)
 *      #warning "Yes, we have EMSIS 0.1.0 or later"
 *   #else
 *      #error "We need EMSIS 0.1.0 or later!"
 *   #endif
 * \endcode
 *
 * @{
 */

/**
 * \file     emsis_version.h
 * \brief    EMSIS Version definitions
 **/

/**
 * \brief   Represent the EMSIS major version
 * \details
 * The EMSIS major version can be used to
 * differentiate between EMSIS major releases.
 * */
#define __EMSIS_VERSION_MAJOR            (0U)

/**
 * \brief   Represent the EMSIS minor version
 * \details
 * The EMSIS minor version can be used to
 * query a EMSIS release update including new features.
 *
 **/
#define __EMSIS_VERSION_MINOR            (1U)

/**
 * \brief   Represent the EMSIS patch version
 * \details
 * The EMSIS patch version can be used to
 * show bug fixes in this package.
 **/
#define __EMSIS_VERSION_PATCH            (0U)
/**
 * \brief   Represent the EMSIS Version
 * \details
 * EMSIS Version format: **MAJOR.MINOR.PATCH**
 * * MAJOR: \ref __EMSIS_VERSION_MAJOR, stored in `bits [31:16]` of \ref __EMSIS_VERSION
 * * MINOR: \ref __EMSIS_VERSION_MINOR, stored in `bits [15:8]` of \ref __EMSIS_VERSION
 * * PATCH: \ref __EMSIS_VERSION_PATCH, stored in `bits [7:0]` of \ref __EMSIS_VERSION
 **/
#define __EMSIS_VERSION                  ((__EMSIS_VERSION_MAJOR << 16U) | (__EMSIS_VERSION_MINOR << 8) | __EMSIS_VERSION_PATCH)

/**
 * @}
 */ /* End of Doxygen Group EMSIS_Core_VersionControl */
#endif
