#!/bin/bash

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


ncol=300
ciklusszam=50000
met_neigh_meret=(0 1)
repl_neigh_meret=(0 1)
phalal=0.1
claimEmpty=20.0
diffuzioGyak=(0 5)
mintavetel_gyak=10
matrixkiiratas_gyak=100
modszer=(1 2 3)
k_p=2
k_1=1
k_2=1
k_3=1.5
i_p=0.2
i_1=0.2
i_2=0.2
i_3=0.2

echo ncol ciklusszam met_neigh_meret repl_neigh_meret phalal claimEmpty diffuzioGyak mintavetel_gyak matrixkiiratas_gyak modszer inicEAP inicEA1 inicEA2 inicEA3 kvaluesP kvalues1 kvalues2 kvalues3 >> $direct/$file

#for i in {0..2}
for kp in ${k_p[@]}
do
	for k1 in ${k_1[@]}
	do
		for k2 in ${k_2[@]}
		do
			for k3 in ${k_3[@]}
			do
				for ip in ${i_p[@]}
				do
					for i1 in ${i_1[@]}
					do
						for i2 in ${i_2[@]}
						do
							for i3 in ${i_3[@]}
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
													for i in {1..1}}
													do
														echo $ncol $ciklusszam ${m} ${r} $phalal ${c} ${d} $mintavetel_gyak $matrixkiiratas_gyak ${k} ${kp} ${k1} ${k2} ${k3} ${ip} ${i1} ${i2} ${i3} >> $direct/$file
													done
												done
											done
										done
									done
								done
							done
						done
					done
				done
			done
		done
	done
done



