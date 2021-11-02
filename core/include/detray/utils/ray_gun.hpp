/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2021 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

#include "detray/tools/intersection_kernel.hpp"
#include "detray/utils/enumerate.hpp"

namespace detray {

/** Intersect all portals in a detector with a given ray.
 *
 * @tparam detector_type the type of the detector
 *
 * @param d the detector
 * @param origin the origin of the ray in global coordinates
 * @param direction the direction of the ray in global coordinater
 *
 * @return a sorted vector of volume indices with the corresponding
 *         intersections of the portals that were encountered
 */
template <typename detector_type>
inline auto shoot_ray(const detector_type &d, const point3 &origin,
                      const point3 &direction) {

    using object_id = typename detector_type::object_id;
    using portal_links = typename detector_type::geometry::portal_links;
    using detray_context = typename detector_type::transform_store::context;

    detray_context default_context;

    track<detray_context> ray = {.pos = origin, .dir = direction};

    std::vector<std::pair<dindex, intersection>> volume_record;

    const auto &transforms = d.transforms(default_context);
    const auto &portals = d.template objects<object_id::e_portal>();
    const auto &masks = d.masks();

    // Loop over volumes
    for (const auto &v : d.volumes()) {
        portal_links links = {};

        // Record the portals the ray intersects
        for (const auto &obj : range(portals, v)) {
            auto pti = intersect(ray, obj, transforms, masks, links);

            // Walk along the direction of intersected masks
            if (pti.status == intersection_status::e_inside &&
                pti.direction == intersection_direction::e_along) {

                // Register the current obj in adjacency list
                // adj_list[v.index()].update();

                // Only look at portals (some geometries do not distinguish)
                if (obj.is_portal()) {
                    volume_record.emplace_back(v.index(), pti);
                }
            }
        }
    }

    // Sort intersections by distance to origin of the ray
    auto sort_path = [&](std::pair<dindex, intersection> a,
                         std::pair<dindex, intersection> b) -> bool {
        return (a.second < b.second);
    };
    std::sort(volume_record.begin(), volume_record.end(), sort_path);

    return volume_record;
};

}  // namespace detray
