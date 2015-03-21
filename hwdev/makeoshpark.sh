#!/bin/bash

filepath=$1/$2
echo $filepath

# Drill file
mv ${filepath}.drl ${filepath}.xln

# Top Layer
mv ${filepath}-F_Cu.gbr ${filepath}.gtl

# Bottom Layer
mv ${filepath}-B_Cu.gbr ${filepath}.gbl

# Top Soldermask
mv ${filepath}-F_Mask.gbr ${filepath}.gts

# Bottom Soldermask
mv ${filepath}-B_Mask.gbr ${filepath}.gbs

# Top Silkscreen
mv ${filepath}-F_SilkS.gbr ${filepath}.gto

# Bottom Silkscreen
mv ${filepath}-B_SilkS.gbr ${filepath}.gbo

# Board outline
mv ${filepath}-Edge_Cuts.gbr ${filepath}.gko

#zip -r ${2} $1
