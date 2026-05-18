#!/bin/bash

############################################################
######## PARAMETEREK - ezket kell majd varialni ############
############################################################

ncol=1000
ciklusszam=10000
met_neigh_meret=(8 16 32)
repl_neigh_meret=1
phalal=0.2
claimEmpty=2.0
diffuzioGyak=0
mintavetel_gyak=10
matrixkiiratas_gyak=5000
modszer=(8 9 4 7 10) # 1: geom mean, 2: minimum, 3: harmonic mean, 4: flat (if any is 0, M=0, else M=1), 5: random uniform U(0,2), 6: Linear flux, 7: Monod, 8: geom mean maximized to 1, 9: minimum maximized to 1, 10: linear flux maximized to 1, 11: antifitness maximized to 1
noEA=4 # not a vector! If you change this, you have to comment in/out the line nested deep in the for loops!
minR=1
antifitness=0.0 # you can make it a vector as well

# replication rates
k_1=1.0
k_2=1.1
k_3=1.2
k_4=1.3

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
#		1: klasszikus, mertani atlag
#		2: minimum (by Gergo)
#		3: reciproc osszegek reciproca (by Sz Andras)
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



# echo ncol ciklusszam met_neigh_meret repl_neigh_meret phalal claimEmpty diffuzioGyak mintavetel_gyak matrixkiiratas_gyak modszer noEA inicEAP inicEA1 inicEA2 kvaluesP kvalues1 kvalues2 >> $direct/$file
echo ncol ciklusszam met_neigh_meret repl_neigh_meret phalal claimEmpty diffuzioGyak mintavetel_gyak matrixkiiratas_gyak modszer noEA antifitness inicEAP inicEA1 inicEA2 inicEA3 inicEA4 kvaluesP kvalues1 kvalues2 kvalues3 kvalues4 >> $direct/$file

for i in {0..1}
do
#for kp in ${k_p[@]}
#do
	for k1 in ${k_1[@]}
	do
		for k2 in ${k_2[@]}
		do
			#for k3 in ${k_3[@]}
			#do
				#for ip in ${i_p[@]}
				#do
					for i1 in ${i_1[@]}
					do
						for i2 in ${i_2[@]}
						do
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
														# echo $ncol $ciklusszam ${m} ${r} $phalal ${c} ${d} $mintavetel_gyak $matrixkiiratas_gyak ${k} ${noEA} ${af} 0 0 ${i1} ${k1} ${i2} ${k2} >> $direct/$file
														echo $ncol $ciklusszam ${m} ${r} $phalal ${c} ${d} $mintavetel_gyak $matrixkiiratas_gyak ${k} ${noEA} ${af} 0 0 ${i1} ${k1} ${i2} ${k2} ${i3} ${k3} ${i4} ${k4} >> $direct/$file
													done
												done
											done
										done
									done
								done
							done
						done
					done
				#done
			#done
		done
	done
#done
done


