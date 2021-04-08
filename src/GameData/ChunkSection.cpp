#include "../../include/GameData/ChunkSection.hpp"

#include <algorithm>
#include <iterator>

ChunkSection::ChunkSection() {
}

ChunkSection::ChunkSection(const glm::ivec3& position, byte size, BlockId blockType)
    : position{position}, size{size}, blockType{blockType} {
}

ChunkSection::ChunkSection(const glm::ivec3& position, byte size, ChunkSection* subsections)
    : position{position}, size{size} {
    this->subsections = new ChunkSection[SUBSECTIONS_COUNT];
    for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
        this->subsections[i] = subsections[i];
    }
}

ChunkSection::ChunkSection(const ChunkSection& other)
    : size{other.size}, position{other.position}, blockType{other.blockType} {

    if (other.subsections == nullptr) {
        this->subsections = nullptr;
    }
    else {
        this->subsections = new ChunkSection[SUBSECTIONS_COUNT];
        for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
            this->subsections[i] = ChunkSection(other.subsections[i]);
        }
    }
}

ChunkSection::ChunkSection(ChunkSection&& other) {
    size = other.size;
    blockType = other.blockType;
    position = other.position;
    subsections = other.subsections;

    other.size = 0;
    other.blockType = BlockId::BLOCK_AIR;
    other.position = glm::ivec3{};
    other.subsections = nullptr;
}

ChunkSection::~ChunkSection() {
    size = 0;
    blockType = BlockId::BLOCK_AIR;
    position = glm::ivec3{};

    if (subsections != nullptr) {
        delete[] subsections;
    }
}

constexpr int ChunkSection::directionToIndex(const glm::ivec3& direction) {
    return (direction.x == 0 ? 0x01 : 0) |
           (direction.y == 0 ? 0x02 : 0) |
           (direction.z == 0 ? 0x04 : 0);
}

constexpr glm::ivec3 ChunkSection::getDirection(int index) {
    return glm::ivec3{
        index & 0x01 ? 0 : 1, // x
        index & 0x02 ? 0 : 1, // y
        index & 0x04 ? 0 : 1  // z
    };
}

// splits the section into 8 subsections of same blockType if size > 1
void ChunkSection::split() {
    // only sections greater than one can be divided
    if (size <= 1)
        return;

    // reserve memory for new subsections
    subsections = new ChunkSection[SUBSECTIONS_COUNT];
    for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
        // calculate position of the new subsection
        glm::ivec3 direction = getDirection(i);
        glm::ivec3 newPosition = position + size / 2 * direction;
        // create the new subsection
        subsections[i] = ChunkSection{newPosition, (byte)(size / 2), blockType};
    }

    // set block type to default
    blockType = BlockId::BLOCK_AIR;
}

// merges subsections together if possible
void ChunkSection::cleanUp() {
    // nothing to clean up if section consists of one block type
    if (!hasSubsections()) {
        return;
    }

    // cleanup subsections and check if all subsections have no subsections
    bool subsectionsHaveNoSubsections = true;
    for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
        subsections[i].cleanUp();
        if (subsections[i].hasSubsections()) {
            subsectionsHaveNoSubsections = false;
        }
    }

    // if all subsections have no subsections
    if (subsectionsHaveNoSubsections) {
        // determine if all subsections consists of the same block type
        BlockId subsectionBlocks = subsections[0].blockType;
        for (int i = 1; i < SUBSECTIONS_COUNT; i++) {
            // return if other block type was found
            if (subsectionBlocks != subsections[i].blockType) {
                return;
            }
        }

        // merge subsections
        delete[] subsections;
        this->blockType = subsectionBlocks;
    }
}

// checks if subsection has subsections
bool ChunkSection::hasSubsections() const {
    return subsections != nullptr;
}

bool ChunkSection::isAir() const {
    return !hasSubsections() && blockType == BlockId::BLOCK_AIR;
}

char* ChunkSection::getData(size_t* length) const {
    if (hasSubsections()) {
        char* subSectionData[SUBSECTIONS_COUNT];
        size_t subSectionDataLengths[SUBSECTIONS_COUNT];
        size_t entireLength = 0;
        for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
            size_t length = 0;
            subSectionData[i] = subsections[i].getData(&length);
            subSectionDataLengths[i] = length;
            entireLength += length;
        }

        *length = entireLength + 5;
        // create buffer
        char* buffer = new char[*length];
        // write node byte
        buffer[0] = 0x01;
        // write length
        for (int i = 0; i < 4; i++) {
            buffer[4 - i] = (char)(entireLength >> (i * 8));
        }

        int pos = 5;
        // write section data
        for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
            std::memcpy(buffer + pos, subSectionData[i], subSectionDataLengths[i]);
            pos += subSectionDataLengths[i];

            // free memory
            delete[] subSectionData[i];
        }

        return buffer;
    }
    else {
        char* buffer = new char[4];
        // write leaf byte
        buffer[0] = 0x00;

        // write section size
        buffer[1] = size;

        // write block type
        buffer[2] = (char)((short)blockType >> 8);
        buffer[3] = (char)blockType;

        *length = 4;
        return buffer;
    }
}

ChunkSection& ChunkSection::operator=(const ChunkSection& other) {
    this->size = other.size;
    this->position = other.position;

    this->blockType = other.blockType;
    if (other.subsections == nullptr) {
        this->subsections = nullptr;
    }
    else {
        this->subsections = new ChunkSection[SUBSECTIONS_COUNT];
        for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
            this->subsections[i] = other.subsections[i];
        }
    }

    return *this;
}
