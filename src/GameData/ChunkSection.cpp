#include "../../include/GameData/ChunkSection.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

SectionPart::SectionPart() {
}

SectionPart::SectionPart(const glm::ivec3& center, int size, BlockId blockType)
    : center{center}, size{size}, blockType{blockType} {
}

SectionPart::SectionPart(const SectionPart& other)
    : size{other.size}, center{other.center}, blockType{other.blockType} {

    if (other.subSections == nullptr) {
        this->subSections = nullptr;
    }
    else {
        this->subSections = new SectionPart[SUBSECTIONS_COUNT];
        for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
            this->subSections[i] = SectionPart(other.subSections[i]);
        }
    }
}

SectionPart::SectionPart(SectionPart&& other) {
    size = other.size;
    blockType = other.blockType;
    center = other.center;
    subSections = other.subSections;

    other.size = 0;
    other.blockType = BlockId::BLOCK_AIR;
    other.center = glm::ivec3{};
    other.subSections = nullptr;
}

SectionPart::~SectionPart() {
    size = 0;
    blockType = BlockId::BLOCK_AIR;
    center = glm::ivec3{};

    if (subSections != nullptr) {
        delete[] subSections;
        subSections = nullptr;
    }
}

constexpr int SectionPart::directionToIndex(const glm::ivec3& direction) {
    return (direction.x < 0 ? 0x01 : 0) |
           (direction.y < 0 ? 0x02 : 0) |
           (direction.z < 0 ? 0x04 : 0);
}

constexpr glm::ivec3 SectionPart::indexToDirection(int index) {
    return glm::ivec3{
        index & 0x01 ? -1 : 1, // x
        index & 0x02 ? -1 : 1, // y
        index & 0x04 ? -1 : 1  // z
    };
}

// returns the block on the specified position
BlockId SectionPart::getBlock(const glm::ivec3& position) const {
    // return block type if no subsections are present
    if (size == 1 || !hasSubsections()) {
        return blockType;
    }

    // find the right subsection relating to the position and get the block
    int index = directionToIndex(position - center);
    return subSections[index].getBlock(position);
}

// sets a block on the specified position
void SectionPart::setBlock(const glm::ivec3& position, BlockId blockType) {
    // size equal to one
    if (size == 1) {
        // replace current block type and return
        this->blockType = blockType;
        return;
    }

    // has no subsections
    if (!hasSubsections()) {
        // split the section into 8 smaller subsections
        split();
    }

    // set block in the right subsection
    int index = directionToIndex(position - center);
    this->subSections[index].setBlock(position, blockType);
}

// splits the section into 8 subsections of same blockType if size > 1
void SectionPart::split() {
    // only sections greater than one can be divided
    if (size == 1)
        return;

    // reserve memory for new subsections
    subSections = new SectionPart[SUBSECTIONS_COUNT];
    for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
        // calculate center of the new subsection
        glm::ivec3 direction = indexToDirection(i);
        glm::ivec3 newCenter = center + size / 2 * direction;
        // create the new subsection
        subSections[i] = SectionPart{newCenter, size / 2, blockType};
    }

    // set block type to default
    blockType = BlockId::BLOCK_AIR;
}

// merges subsections together if possible
void SectionPart::cleanUp() {
    // nothing to clean up if section consists of one block type
    if (!hasSubsections()) {
        return;
    }

    // cleanup subsections and check if all subsections have no subsections
    bool subsectionsHaveNoSubsections = true;
    for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
        subSections[i].cleanUp();
        if (subSections[i].hasSubsections()) {
            subsectionsHaveNoSubsections = false;
        }
    }

    // if all subsections have no subsections
    if (subsectionsHaveNoSubsections) {
        // determine if all subsections consists of the same block type
        BlockId subsectionBlocks = subSections[0].blockType;
        for (int i = 1; i < SUBSECTIONS_COUNT; i++) {
            // return if other block type was found
            if (subsectionBlocks != subSections[i].blockType) {
                return;
            }
        }

        // merge subsections
        delete[] subSections;
        subSections = nullptr;
        this->blockType = subsectionBlocks;
    }
}

// checks if subsection has subsections
bool SectionPart::hasSubsections() const {
    return !(subSections == nullptr);
}

SectionPart& SectionPart::operator=(const SectionPart& other) {
    this->size = other.size;
    this->center = other.center;

    this->blockType = other.blockType;
    if (other.subSections == nullptr) {
        this->subSections = nullptr;
    }
    else {
        this->subSections = new SectionPart[SUBSECTIONS_COUNT];
        for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
            this->subSections[i] = other.subSections[i];
        }
    }

    return *this;
}

void SectionPart::print(int indent) const {
    for (int i = 0; i < indent; i++) {
        std::cout << "\t";
    }
    std::cout << "Size: " << size;
    if (!hasSubsections()) {
        std::cout << " BlockId: " << (int)blockType << std::endl;
    }
    else {
        std::cout << std::endl;
        for (int i = 0; i < SUBSECTIONS_COUNT; i++) {
            subSections[i].print(indent + 1);
        }
    }
}

void ChunkSection::setBlock(const glm::ivec3& position, BlockId blockType) {
    blocks->setBlock(position, blockType);
}

BlockId ChunkSection::getBlock(const glm::ivec3& position) const {
    return blocks->getBlock(position);
}
