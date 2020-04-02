//
// Created by 2ToThe10th on 02.04.2020.
//

#include "GraphicMap.h"


namespace Client::GraphicAgent {

GraphicMap::GraphicMap(Map &main_map)
    : main_map_(main_map), last_syncronize(std::numeric_limits<size_t>::max()) {
}

}