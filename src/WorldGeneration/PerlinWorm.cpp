#include "../../include/WorldGeneration/PerlinWorm.h"
#include "../../include/Utility.h"
#include "../../include/Configuration.h"
#include <iostream>

void PerlinWorm::generateWorm(noise::module::Perlin &wormNoise, glm::vec3 head, int segmentsCount, float maxLength) {
    float maxSegmentLength = maxLength / segmentsCount;
    glm::vec3 phiOffset = glm::vec3(1, 0, 1);
    glm::vec3 noiseDirection = glm::vec3(0, 1, 0);
    glm::vec3 currPos = head;
    
    for (int i = 0; i < segmentsCount; i++) {
        // spherical coords
        float segmentLength = maxSegmentLength;

        glm::vec3 noisePointTheta = head + (float)i * noiseDirection;        
        glm::vec3 noisePointPhi = head + phiOffset + (float)i * noiseDirection;

        // calc segment angles
        float theta, phi;
        theta = wormNoise.GetValue(noisePointTheta.x, noisePointTheta.y, noisePointTheta.z);
        phi = wormNoise.GetValue(noisePointPhi.x, noisePointPhi.y, noisePointPhi.z) * 2.0f;
        std::cout << "theta: " << theta << " phi: " << phi << std::endl;

        // calc cartesian coords
        glm::vec3 segment;
        segment.x = segmentLength * sin(theta) * cos(phi);
        segment.y = segmentLength * sin(theta) * sin(phi);
        segment.z = segmentLength * cos(theta);

        // stop worm generation if coords go outside world range
        if(currPos.y + segment.y >= CHUNK_HEIGHT) {
            break;
        }

        // store segment
        segments.push_back(round(segment));
    }
}

PerlinWorm::PerlinWorm(noise::module::Perlin& wormNoise, glm::vec3 head, int segmentsCount, float maxLength) 
    : segmentsCount(segmentsCount), head(head) {
    generateWorm(wormNoise, head, segmentsCount, maxLength);
}

void DivideWorm(PerlinWorm worm, std::vector<WormPart>& parts) {
    int index = 0;
    glm::vec2 chunk; glm::vec3 localCoords;
    chunk = GetChunk(worm.head, localCoords);

    glm::vec3 pos = worm.head;

    WormPart chunkWorm = {};
    chunkWorm.head = localCoords;
    chunkWorm.index = index;
    chunkWorm.chunk = chunk;

    for (int i = 0; i < worm.segmentsCount; i++) {
        glm::vec3 segment = worm.segments[i];
        glm::vec3 localPos = GetChunkCoords(pos);

        if(InChunk(localPos + segment)) {
            pos += segment;
            chunkWorm.segments.push_back(segment);
        }
        else {                 
            float rx = (ceil(pos.x / CHUNK_SIZE) * CHUNK_SIZE - (1 + pos.x)) / segment.x;
            float rz = (ceil(pos.z / CHUNK_SIZE) * CHUNK_SIZE - (1 + pos.z)) / segment.z;
            float r_min = min(rx, rz);

            glm::vec3 end = localPos + r_min * segment;
            chunkWorm.segments.push_back(end - localPos);
            chunkWorm.segmentsCount = chunkWorm.segments.size();
            parts.push_back(chunkWorm);

            // create new chunkWorm
            chunkWorm = {};
            chunkWorm.index = ++index;

            float tx = rx + 1.0f / segment.x;
            float tz = rz + 1.0f / segment.z;
            float t_min = min(tx, tz);

            glm::vec3 newHead = pos + t_min * segment;
            chunkWorm.chunk = GetChunk(newHead, chunkWorm.head);
            chunkWorm.segments.push_back((1 - t_min) * segment);
            pos += segment;
        }
    }
}