#pragma once
#include "core.hpp"

namespace pharma {

class Vehicle {
protected:
    std::string id_;
    std::string plate_;
    std::string model_;
    double capacityKg_;
    double capacityM3_;
    bool available_;
    std::string driverId_;
public:
    Vehicle(std::string id, std::string plate, std::string model,
            double capKg, double capM3);
    virtual ~Vehicle() = default;

    bool isAvailable() const { return available_; }
    void setAvailable(bool v) { available_ = v; }
    bool assignDriver(const std::string& driverId);
    virtual bool canTransport(const std::string& cargoType) const;
    virtual double maxRange() const = 0;
    const std::string& id() const { return id_; }
    double capacityKg() const { return capacityKg_; }
};

class Truck : public Vehicle {
protected:
    int axles_;
public:
    Truck(std::string id, std::string plate, std::string model, double capKg,
          double capM3, int axles);
    double maxRange() const override { return 1200.0; }
    bool canTransport(const std::string& cargoType) const override;
};

class Van : public Vehicle {
public:
    Van(std::string id, std::string plate, std::string model, double capKg,
        double capM3);
    double maxRange() const override { return 500.0; }
};

class RefrigeratedTruck : public Truck {
    double minTemp_;
    double maxTemp_;
    bool coolingActive_;
public:
    RefrigeratedTruck(std::string id, std::string plate, std::string model,
                      double capKg, double capM3, int axles, double minT,
                      double maxT);
    bool checkColdChain(double t) const;
    bool canTransport(const std::string& cargoType) const override;
};

class Route {
    std::string id_;
    std::vector<Address> stops_;
    double totalDistanceKm_;
    int estimatedMinutes_;
    std::string vehicleId_;
public:
    Route(std::string id, std::vector<Address> stops, double distanceKm,
          int minutes);
    double estimateFuel(double consumptionPer100Km) const;
    bool isFeasible(const Vehicle& v) const;
    void addStop(const Address& a);
    const std::string& id() const { return id_; }
};

} 