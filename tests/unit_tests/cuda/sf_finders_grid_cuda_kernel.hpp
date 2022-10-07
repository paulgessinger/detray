/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2021 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#if defined(array)
#include "detray/plugins/algebra/array_definitions.hpp"
#elif defined(eigen)
#include "detray/plugins/algebra/eigen_definitions.hpp"
#elif defined(smatrix)
#include "detray/plugins/algebra/smatrix_definitions.hpp"
#elif defined(vc_array)
#include "detray/plugins/algebra/vc_array_definitions.hpp"
#endif

// detray core
#include "detray/masks/masks.hpp"
#include "detray/surface_finders/grid/axis.hpp"
#include "detray/surface_finders/grid/grid.hpp"
#include "detray/surface_finders/grid/grid_builder.hpp"
#include "detray/surface_finders/grid/populator.hpp"
#include "detray/surface_finders/grid/serializer.hpp"

#pragma once

namespace {

// type definitions
using size_type = __plugin::size_type;
using vector3 = __plugin::vector3<detray::scalar>;
using point3 = __plugin::point3<detray::scalar>;

}  // namespace

namespace detray {

using namespace n_axis;

static constexpr std::size_t n_points = 3;
static constexpr bool is_owning = true;

// Coordinate system definitions (all data-owning)

// 3D cartesian coordinate axes with closed bin boundaries and regular binning
template <typename containers = host_container_types>
using cartesian_3D =
    coordinate_axes<cuboid3D<>::axes<shape::e_closed>, is_owning, containers>;

// 2D polar coordinate axes with closed bin boundaries, irregular binning in r
// and regular binning in phi
template <typename containers = host_container_types>
using polar_ir =
    coordinate_axes<ring2D<>::axes<shape::e_closed, irregular, regular>,
                    is_owning, containers>;

// 2D polar coordinate axes with closed bin boundaries and regular binning
template <typename containers = host_container_types>
using polar =
    coordinate_axes<ring2D<>::axes<shape::e_closed>, is_owning, containers>;

// host and device grid definitions
using host_grid3_replace =
    grid<cartesian_3D<>, point3, simple_serializer, replacer>;

using device_grid3_replace = grid<cartesian_3D<device_container_types>, point3,
                                  simple_serializer, replacer>;

using host_grid2_replace_ci =
    grid<polar_ir<>, point3, simple_serializer, replacer>;

using device_grid2_replace_ci =
    grid<polar_ir<device_container_types>, point3, simple_serializer, replacer>;

using host_grid2_complete =
    grid<polar<>, point3, simple_serializer, completer<n_points>>;

using device_grid2_complete = grid<polar<device_container_types>, point3,
                                   simple_serializer, completer<n_points>>;

using host_grid2_attach =
    grid<polar<>, point3, simple_serializer, regular_attacher<n_points>>;

using device_grid2_attach = grid<polar<device_container_types>, point3,
                                 simple_serializer, regular_attacher<n_points>>;

using const_host_grid2_attach =
    grid<polar<>, const point3, simple_serializer, regular_attacher<n_points>>;

using const_device_grid2_attach =
    grid<polar<device_container_types>, const point3, simple_serializer,
         regular_attacher<n_points>>;

/// test function for replace populator
void grid_replace_test(host_grid3_replace::view_type grid_view,
                       std::size_t dim_x, std::size_t dim_y, std::size_t dim_z);

/// test function for replace populator with circular and irregular axis
void grid_replace_ci_test(host_grid2_replace_ci::view_type grid_view,
                          std::size_t dim_x, std::size_t dim_y);

// test function for complete populator
void grid_complete_test(host_grid2_complete::view_type grid_view,
                        std::size_t dim_x, std::size_t dim_y);

// read test function for grid with attach populator
void grid_attach_test(host_grid2_attach::view_type grid_view, std::size_t dim_x,
                      std::size_t dim_y);

// read test function for grid with attach populator
void grid_attach_read_test(const_host_grid2_attach::view_type grid_view,
                           std::size_t dim_x, std::size_t dim_y);

}  // namespace detray
