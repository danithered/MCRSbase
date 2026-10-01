#!/bin/bash

############################################################
######## PARAMETEREK - ezket kell majd varialni ############
############################################################

repeats=1 # number of repeats of the same parameter set
ncol=1000 # number of columns or rows of the grid. Number of cells = ncol * ncol 
ciklusszam=10000 # maximal lenght of the simulations
met_neigh_meret=(8 16 32) # size of the metabolic neighbourhood
repl_neigh_meret=1 # size of the replicator neighbourhood
phalal=0.1 # probabilty of degradation
claimEmpty=0.1 
diffuzioGyak=3 # frequency of replicator mobility steps
mintavetel_gyak=10 # time interval of output rows
matrixkiiratas_gyak=5000 # time interval of saving the whole grid
modszer=(4 7 8 9 10) # metabolic function applied. 1: geom mean, 2: minimum, 3: harmonic mean, 4: flat (if any is 0, M=0, else M=1), 5: random uniform U(0,2), 6: Linear flux, 7: Monod, 8: geom mean maximized to 1, 9: minimum maximized to 1, 10: linear flux maximized to 1, 11: antifitness maximized to 1
noEA=4 # not a vector! If you change this, you have to comment in/out the line nested deep in the for loops (look for 2 lines starting with "######")!
antifitness=0.0 # the antifitness value applied in metabolic function 11

# replication rates
k_1=1.0
k_2=1.05
k_3=1.1
k_4=1.15

# initial frequencies
i_1=0.2
i_2=0.2
i_3=0.2
i_4=0.2


#######################################################################################
########### working parts - change it if only you know what you are doing! ############
#######################################################################################

#	1: ncol: alapmatrix oszlopainak szama
#	2: ciklusszam: milyen sokaig fut a program
#	3: met_neigh_meret: metabolikus szomszedsag merete
#	4: repl_neigh_meret: metabolikus szomszedsag merete
#	5: phalal: extinkcio valsege
#	6: claimEmpty: az uresen maradas claim-ja
#	7: diffuzioGyak: milyen gyakran kovetekzik be diff esemeny
#	8: mintavetel_gyak: milyen gyakran irjon ki atlagadatokat: 0 soha, 1 minden generacioban, 2 minden 2. generacioban
#	9: matrixkiiratas_gyak: milyen gyakran irja ki a teljes matrixot
#	10: modszer: melyik fuggvennyel szamitsa ki a metabolizmus hatekonysagat
#		1: geom mean
#		2: minimum
#		3: harmonic mean
#		4: flat (if any is 0, M=0, else M=1)
#		5: random uniform U(0,2)
#		6: Linear flux
#		7: Monod
#		8: geom mean maximized to 1
#		9: minimum maximized to 1
#		10: linear flux maximized to 1
#		11: antifitness maximized to 1
#	11: NOEA
#	 
#	... EA adatok ...
#	(parazita, E1, E1 .... En) iniciacios gyakorisag
#	(parazita, E1, E1 .... En) k
#	 
#	azon: egyedi azonosito
#	

direct="IN"
file="param"
if [ ! -d  $direct ]; then
	mkdir IN
fi

if [ -e $direct/$file ]; then
	cp $direct/$file $direct/$file$(date +"%T")
	rm $direct/$file
fi 
touch $direct/$file


###### comment in the next line if noEA is 2 and comment out the one following it
# echo ncol ciklusszam met_neigh_meret repl_neigh_meret phalal claimEmpty diffuzioGyak mintavetel_gyak matrixkiiratas_gyak modszer noEA inicEAP inicEA1 inicEA2 kvaluesP kvalues1 kvalues2 >> $direct/$file
echo ncol ciklusszam met_neigh_meret repl_neigh_meret phalal claimEmpty diffuzioGyak mintavetel_gyak matrixkiiratas_gyak modszer noEA antifitness inicEAP kvaluesP inicEA1 kvalues1 inicEA2 kvalues2 inicEA3 kvalues3 inicEA4 kvalues4 >> $direct/$file

for ((i=0; i<$repeats; i++))
do
#for kp in ${k_p[@]}
#do
	for k1 in ${k_1[@]}
	do
		for k2 in ${k_2[@]}
		do
			for k3 in ${k_3[@]}
			do
				for pdeg in ${phalal[@]}
				do
					for k4 in ${k_4[@]}
					do
						#for i2 in ${i_2[@]}
						#do
							for af in ${antifitness[@]}
							do
								for m in ${met_neigh_meret[@]}
								do	
									for k in ${modszer[@]}
									do
										for c in ${claimEmpty[@]}
										do
											for d in ${diffuzioGyak[@]}
											do
												for r in ${repl_neigh_meret[@]}
												do
													for i2 in {1..1}}
													do
###### comment in the next line if noEA is 2 and comment out the one following it
														# echo $ncol $ciklusszam ${m} ${r} $pdeg ${c} ${d} $mintavetel_gyak $matrixkiiratas_gyak ${k} ${noEA} ${af} 0 0 ${i1} ${k1} ${i2} ${k2} >> $direct/$file
														echo $ncol $ciklusszam ${m} ${r} $pdeg ${c} ${d} $mintavetel_gyak $matrixkiiratas_gyak ${k} ${noEA} ${af} 0 0 ${i_1} ${k1} ${i_2} ${k2} ${i_3} ${k3} ${i_4} ${k4} >> $direct/$file
													done
												done
											done
										done
									done
								done
							done
						#done
					done
				done
			done
		done
	done
#done
done


