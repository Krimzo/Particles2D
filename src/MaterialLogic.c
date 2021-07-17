#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MyStructs.h"
#include "Materials.h"
#include "MyFunctions.h"


// Particle logic for sand
void MaterialLogicSand(POINT2D location)
{
    int x = location.x;
    int y = location.y;
    if (!(particles + (y * windowWidth) + x)->updated) {
        // First, check one down
        if (y > 0 && (ColorCompare(ColorAtPoint((POINT2D) { x, y - 1 }), backgroundColor) || ColorCompare(ColorAtPoint((POINT2D) { x, y - 1 }), _MaterialWater))) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x, y - 1 });
            (particles + ((y - 1) * windowWidth) + x)->updated = 1;
        }
        // The bottom-right
        else if (y > 0 && x < windowWidth - 1 && (ColorCompare(ColorAtPoint((POINT2D) { x + 1, y - 1 }), backgroundColor) || ColorCompare(ColorAtPoint((POINT2D) { x + 1, y - 1 }), _MaterialWater))) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x + 1, y - 1 });
            (particles + ((y - 1) * windowWidth) + (x + 1))->updated = 1;
        }
        // The bottom-left
        else if (y > 0 && x > 0 && (ColorCompare(ColorAtPoint((POINT2D) { x - 1, y - 1 }), backgroundColor) || ColorCompare(ColorAtPoint((POINT2D) { x - 1, y - 1 }), _MaterialWater))) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x - 1, y - 1 });
            (particles + ((y - 1) * windowWidth) + (x - 1))->updated = 1;
        }
    }
}

// Particle logic for water
void MaterialLogicWater(POINT2D location)
{
    int x = location.x;
    int y = location.y;
    if (!(particles + (y * windowWidth) + x)->updated) {
        // First, check one down
        if (y > 0 && ColorCompare(ColorAtPoint((POINT2D) { x, y - 1 }), backgroundColor)) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x, y - 1 });
            (particles + ((y - 1) * windowWidth) + x)->updated = 1;
        }
        // The bottom-right
        else if (y > 0 && x < windowWidth - 1 && ColorCompare(ColorAtPoint((POINT2D) { x + 1, y - 1 }), backgroundColor)) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x + 1, y - 1 });
            (particles + ((y - 1) * windowWidth) + (x + 1))->updated = 1;
        }
        // The bottom-left
        else if (y > 0 && x > 0 && ColorCompare(ColorAtPoint((POINT2D) { x - 1, y - 1 }), backgroundColor)) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x - 1, y - 1 });
            (particles + ((y - 1) * windowWidth) + (x - 1))->updated = 1;
        }
        // The right
        else if (x < windowWidth - 1 && ColorCompare(ColorAtPoint((POINT2D) { x + 1, y }), backgroundColor)) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x + 1, y });
            (particles + (y * windowWidth) + (x + 1))->updated = 1;
        }
        // The left
        else if (x > 0 && ColorCompare(ColorAtPoint((POINT2D) { x - 1, y }), backgroundColor)) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x - 1, y });
            (particles + (y * windowWidth) + (x - 1))->updated = 1;
        }
    }
}

// Particle logic for rock
void MaterialLogicRockParticle(POINT2D location)
{
    int x = location.x;
    int y = location.y;
    if (!(particles + (y * windowWidth) + x)->updated) {
        // First, check one down
        if (y > 0 && (ColorCompare(ColorAtPoint((POINT2D) { x, y - 1 }), backgroundColor) || ColorCompare(ColorAtPoint((POINT2D) { x, y - 1 }), _MaterialWater) || ColorCompare(ColorAtPoint((POINT2D) { x, y - 1 }), _MaterialSand))) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x, y - 1 });
            (particles + ((y - 1) * windowWidth) + x)->updated = 1;
        }
        // The bottom-right
        else if (y > 0 && x < windowWidth - 1 && (ColorCompare(ColorAtPoint((POINT2D) { x + 1, y - 1 }), backgroundColor) || ColorCompare(ColorAtPoint((POINT2D) { x + 1, y - 1 }), _MaterialWater) || ColorCompare(ColorAtPoint((POINT2D) { x + 1, y - 1 }), _MaterialSand))) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x + 1, y - 1 });
            (particles + ((y - 1) * windowWidth) + (x + 1))->updated = 1;
        }
        // The bottom-left
        else if (y > 0 && x > 0 && (ColorCompare(ColorAtPoint((POINT2D) { x - 1, y - 1 }), backgroundColor) || ColorCompare(ColorAtPoint((POINT2D) { x - 1, y - 1 }), _MaterialWater) || ColorCompare(ColorAtPoint((POINT2D) { x - 1, y - 1 }), _MaterialSand))) {
            SwitchParticles((POINT2D) { x, y }, (POINT2D) { x - 1, y - 1 });
            (particles + ((y - 1) * windowWidth) + (x - 1))->updated = 1;
        }
    }
}