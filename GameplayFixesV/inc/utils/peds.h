#pragma once
#include "utils\player.h"

class PedOption : public IOption {
private:
	iniValue m_iniFlag;
	std::function<void(Ped ped)> m_option;
	Ped* m_ped = nullptr;
	std::string m_name;

public:
	PedOption(iniValue iniFlag, std::function<void(Ped ped)> action, Ped* ped, const std::string& name)
		: m_iniFlag(iniFlag), m_option(action), m_ped(ped), m_name(name) {
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
		m_option(*m_ped);
	}

	std::string GetName() const override {
		return m_name;
	}
};

void RegisterPedOptions();
void UpdatePedsPool();