#include "Bank.h"
#include "BankableMIDIInputAddressable.hpp"
#include "BankableMIDIOutputAddressable.hpp"
#include "BankableMIDIOutput.hpp"

Bank::Bank(uint8_t tracksPerBank) : tracksPerBank(tracksPerBank) {
    if (tracksPerBank == 0) {
        DEBUGFN(F("Error: A Bank must have a non-zero number of tracks."));
        FATAL_ERROR();
    }
}

void Bank::add(BankableMIDIInputAddressable *bankable, bankType type) {
    add(*bankable, type);
}

void Bank::add(BankableMIDIInputAddressable &bankable, bankType type) {
    LinkedList::append(&bankable, first, last);
    bankable.setBank(this, type);
}

void Bank::add(BankableMIDIOutput &bankable) {
    bankable.setBank(this);
}

void Bank::add(BankableMIDIOutputAddressable &bankable, bankType type) {
    bankable.setBank(this, type);
}

void Bank::setBankSetting(uint8_t bankSetting) {
    this->bankSetting = bankSetting;
    first->onBankSettingChangeAll();
}

uint8_t Bank::getBankSetting() const { return bankSetting; }

uint8_t Bank::getTracksPerBank() const { return tracksPerBank; }

void Bank::remove(BankableMIDIInputAddressable *bankable) {
    bankable->removeBank();
    LinkedList::remove(bankable, first, last);
}
