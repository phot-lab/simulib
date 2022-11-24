/**
 * Copyright (c) 2022 Beijing Jiaotong University
 * OpticaLab is licensed under [Open Source License].
 * You can use this software according to the terms and conditions of the [Open Source License].
 * You may obtain a copy of [Open Source License] at: [https://open.source.license/]
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the [Open Source License] for more details.
 */
/**
 * Author: Chunyu Li
 * Created: 2022/4/26
 * Supported by: National Key Research and Development Program of China
 */

/**
 * RxOption struct
 */

#ifndef RXFRONTEND_H
#define RXFRONTEND_H

struct RxOption {
    int dcum               = INT_MIN;  // post compensating fiber accumulated dispersion [ps/nm]. The fiber is inserted before the optical filter.
    double obw             = 1.8;      // OBPF bandwidth normalized to SYMBRATE.
    double ebw             = 0.5;      // Electrical filter bandwidth normalized to symbrate
    double mzDelay         = 1;        // default interferometer delay: 1 symbol
    double filterParameter = 0;        // optical filter extra parameters (optional, see MYFILTER)
    string modFormat;                  // modulation format
    string ofType;                     // optical filter (OBPF) type (see MYFILTER) (used in receiver module)
    string syncType;                   // time-recovery method
    string efType;                     // Electrical filter type
};

#endif  // RXFRONTEND_H
