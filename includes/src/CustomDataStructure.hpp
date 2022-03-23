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
 * Created: 2022/3/21
 * Supported by: National Key Research and Development Program of China
 */

#ifndef CUSTOM_DATA_STRUCTURE_H
#define CUSTOM_DATA_STRUCTURE_H

#include <map>
#include <memory>
#include <string>

class va_map {
private:
    std::map<std::string, std::shared_ptr<void>> normal_map;

public:
    template<typename T>
    void put(const std::string &key, const T &value) {
        std::shared_ptr<T> ptr = std::make_shared<T>(value);
        normal_map[key]        = ptr;
    }

    template<typename T>
    T get(const std::string &key) {
        std::shared_ptr<void> ptr = normal_map.at(key);
        return *(static_pointer_cast<T>(ptr));
    }

    bool remove(const std::string &key) {
        return normal_map.erase(key);
    }

    bool contains(const std::string &key) {
        return normal_map.count(key) == 1;
    }
};

#endif  // CUSTOM_DATA_STRUCTURE_H
