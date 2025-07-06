#pragma once
#include "globals.h"
#include <algorithm>
#include <vector>
#include <string>

union uValue {
	int Int;
	unsigned int Uns;
	float Float;
	const char* String;
	bool Bool;
	uValue() : Int(0) {}
	uValue(int i) : Int(i) {}
	uValue(unsigned int u) : Uns(u) {}
	uValue(float f) : Float(f) {}
	uValue(const char* s) : String(s) {}
	uValue(bool b) : Bool(b) {}
};

enum iniValueType {
	IVT_INT,
	IVT_UNSIGNED_INT,
	IVT_FLOAT,
	IVT_STRING,
	IVT_BOOL
};

struct iniValue {
	uValue value;
	iniValueType type;
	iniValue(int i) { value.Int = i; type = IVT_INT; }
	iniValue(unsigned int u) { value.Uns = u; type = IVT_UNSIGNED_INT; }
	iniValue(float f) { value.Float = f; type = IVT_FLOAT; }
	iniValue(const char* s) { value.String = s; type = IVT_STRING; }
	iniValue(bool b) { value.Bool = b; type = IVT_BOOL; }
};

class IOption {
public:
	virtual ~IOption() {} // virtual destructor
	virtual bool IsEnabled() const = 0;
	virtual void Apply() = 0;
	virtual std::string GetName() const = 0;
};

class PlayerOption : public IOption {
private:
	iniValue m_iniFlag;
	std::function<void()> m_option;
	std::string m_name;

public:
	PlayerOption(iniValue iniFlag, std::function<void()> action, const std::string& name)
		: m_iniFlag(iniFlag), m_option(action), m_name(name) {
	}

	bool IsEnabled() const override {
		switch (m_iniFlag.type) {
		case IVT_INT:
			return (m_iniFlag.value.Int > 0);
			break;
		case IVT_UNSIGNED_INT:
			return (m_iniFlag.value.Uns > 0);
			break;
		case IVT_FLOAT:
			return (m_iniFlag.value.Float >= 0.0f);
			break;
		case IVT_STRING:
			return (m_iniFlag.value.String == nullptr || m_iniFlag.value.String == "");
			break;
		case IVT_BOOL:
			return m_iniFlag.value.Bool;
			break;
		}
		return false;
	}

	void Apply() override {
		m_option();
	}

	std::string GetName() const override {
		return m_name;
	}
};

class OptionManager {
private:
	// std::unique_ptr ensures proper memory management for polymorphic objects.
	std::vector<std::unique_ptr<IOption>> m_options;

public:
	// Allows adding any object that implements the IOption interface.
	bool RegisterOption(std::unique_ptr<IOption> option) {
		if (option->IsEnabled()) {
			m_options.push_back(std::move(option)); // std::move transfers ownership
			return true;
		}
		return false;
	}

	void UnregisterAllOptions() {
		m_options.clear();
		return;
	}

	void ApplyOptions() const {
		for (const auto& option_ptr : m_options) { // option_ptr is std::unique_ptr<IOption>&
			option_ptr->Apply();
		}
	}
};

void RegisterPlayerOptions();
void UpdatePlayerOptions();
void RefreshIni();