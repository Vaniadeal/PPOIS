#include "transport.hpp"

namespace pharma {

Vehicle::Vehicle(std::string id, std::string plate, std::string model,
                 double capKg, double capM3)
    : id_(std::move(id)), plate_(std::move(plate)), model_(std::move(model)),
      capacityKg_(capKg), capacityM3_(capM3), available_(true) {}

bool Vehicle::assignDriver(const std::string& driverId) {
    if (!available_) throw VehicleNotAvailableException(plate_);
    driverId_ = driverId; return true;
}
bool Vehicle::canTransport(const std::string&) const { return true; }

Truck::Truck(std::string id, std::string plate, std::string model, double capKg,
             double capM3, int axles)
    : Vehicle(std::move(id), std::move(plate), std::move(model), capKg, capM3),
      axles_(axles) {}

bool Truck::canTransport(const std::string& cargoType) const {
    return cargoType != "cold";
}

Van::Van(std::string id, std::string plate, std::string model, double capKg,
         double capM3)
    : Vehicle(std::move(id), std::move(plate), std::move(model), capKg, capM3) {}

RefrigeratedTruck::RefrigeratedTruck(std::string id, std::string plate,
                                     std::string model, double capKg,
                                     double capM3, int axles, double minT,
                                     double maxT)
    : Truck(std::move(id), std::move(plate), std::move(model), capKg, capM3, axles),
      minTemp_(minT), maxTemp_(maxT), coolingActive_(true) {}

bool RefrigeratedTruck::checkColdChain(double t) const {
    if (t < minTemp_ || t > maxTemp_)
        throw ColdChainViolationException(t, minTemp_, maxTemp_);
    return true;
}
bool RefrigeratedTruck::canTransport(const std::string& cargoType) const {
    return cargoType == "cold" || cargoType == "vaccine";
}

Route::Route(std::string id, std::vector<Address> stops, double distanceKm,
             int minutes)
    : id_(std::move(id)), stops_(std::move(stops)),
      totalDistanceKm_(distanceKm), estimatedMinutes_(minutes) {}

double Route::estimateFuel(double consumptionPer100Km) const {
    return totalDistanceKm_ / 100.0 * consumptionPer100Km;
}
bool Route::isFeasible(const Vehicle& v) const {
    return totalDistanceKm_ <= v.maxRange();
}
void Route::addStop(const Address& a) { stops_.push_back(a); }

} 