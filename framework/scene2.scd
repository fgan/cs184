(Material matRED
	(color 0.75 0.25 0.25 )
	(ka 0.5 ) (kd 0.6 ) (ks 0.1 ) (ksp 150 ) (ksm 1 ) (kt 0 ) (ktn 1 ))
(Material matBLUE
	(color 0.25 0.25 0.75 )
	(ka 0.5 ) (kd 0.6 ) (ks 0.1 ) (ksp 20 ) (ksm 1 ) (kt 0 ) (ktn 1 ))
(Material matWHITE
	(color 0.75 0.75 0.75 )
	(ka 0.5 ) (kd 0.6 ) (ks 0.1 ) (ksp 40 ) (ksm 1 ) (kt 0 ) (ktn 1 ))
(Material matGlass
	(color 0.75 0.75 0.75 )
	(ka 0.0 ) (kd 0.0 ) (ks 0.2 ) (ksp 40 ) (ksm 1 ) (kt 0 ) (ktn 1 ))
(Light amb (type 3 ) (color 0.2 0.2 0.2 ) )
(Light pt1 (type 1 ) (color 20.8 20.8 20.8 )  (falloff 1 )  (deaddistance 0.1 ) )
(Camera cam
	(perspective 1 )
	(l -1 ) (r 1 ) (b -1 ) (t 1 ) (n 3 )) #no far plane
(Sphere spWALL (radius 100000.0 ) (material matWHITE ))
(Sphere spLEFT (radius 100000.0 ) (material matBLUE ))
(Sphere spRIGHT (radius 100000.0 ) (material matRED ))
(Sphere spGlass (radius 4 ) (material matGlass ))
(Material helixMat0
	(color 0.0 1.0 0.335001069639 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_0 (radius 1.5 ) (material helixMat0 ))
(Material helixMat1
	(color 0.025 0.975 0.717533103556 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_1 (radius 1.5 ) (material helixMat1 ))
(Material helixMat2
	(color 0.05 0.95 0.549281656954 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_2 (radius 1.5 ) (material helixMat2 ))
(Material helixMat3
	(color 0.075 0.925 0.881242797898 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_3 (radius 1.5 ) (material helixMat3 ))
(Material helixMat4
	(color 0.1 0.9 0.425878975346 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_4 (radius 1.5 ) (material helixMat4 ))
(Material helixMat5
	(color 0.125 0.875 0.480437088622 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_5 (radius 1.5 ) (material helixMat5 ))
(Material helixMat6
	(color 0.15 0.85 0.590895658474 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_6 (radius 1.5 ) (material helixMat6 ))
(Material helixMat7
	(color 0.175 0.825 0.0731546553435 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_7 (radius 1.5 ) (material helixMat7 ))
(Material helixMat8
	(color 0.2 0.8 0.309454548442 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_8 (radius 1.5 ) (material helixMat8 ))
(Material helixMat9
	(color 0.225 0.775 0.172024365869 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_9 (radius 1.5 ) (material helixMat9 ))
(Material helixMat10
	(color 0.25 0.75 0.686141289847 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_10 (radius 1.5 ) (material helixMat10 ))
(Material helixMat11
	(color 0.275 0.725 0.0576275245735 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_11 (radius 1.5 ) (material helixMat11 ))
(Material helixMat12
	(color 0.3 0.7 0.16627936434 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_12 (radius 1.5 ) (material helixMat12 ))
(Material helixMat13
	(color 0.325 0.675 0.98830606125 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_13 (radius 1.5 ) (material helixMat13 ))
(Material helixMat14
	(color 0.35 0.65 0.796873195756 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_14 (radius 1.5 ) (material helixMat14 ))
(Material helixMat15
	(color 0.375 0.625 0.310428976519 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_15 (radius 1.5 ) (material helixMat15 ))
(Material helixMat16
	(color 0.4 0.6 0.118371161662 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_16 (radius 1.5 ) (material helixMat16 ))
(Material helixMat17
	(color 0.425 0.575 0.110225027492 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_17 (radius 1.5 ) (material helixMat17 ))
(Material helixMat18
	(color 0.45 0.55 0.579424409589 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_18 (radius 1.5 ) (material helixMat18 ))
(Material helixMat19
	(color 0.475 0.525 0.797722409553 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_19 (radius 1.5 ) (material helixMat19 ))
(Material helixMat20
	(color 0.5 0.5 0.190442898758 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_20 (radius 1.5 ) (material helixMat20 ))
(Material helixMat21
	(color 0.525 0.475 0.747567229582 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_21 (radius 1.5 ) (material helixMat21 ))
(Material helixMat22
	(color 0.55 0.45 0.877417010712 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_22 (radius 1.5 ) (material helixMat22 ))
(Material helixMat23
	(color 0.575 0.425 0.894400281756 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_23 (radius 1.5 ) (material helixMat23 ))
(Material helixMat24
	(color 0.6 0.4 0.146916859426 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_24 (radius 1.5 ) (material helixMat24 ))
(Material helixMat25
	(color 0.625 0.375 0.273021618904 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_25 (radius 1.5 ) (material helixMat25 ))
(Material helixMat26
	(color 0.65 0.35 0.884016177378 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_26 (radius 1.5 ) (material helixMat26 ))
(Material helixMat27
	(color 0.675 0.325 0.946384528027 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_27 (radius 1.5 ) (material helixMat27 ))
(Material helixMat28
	(color 0.7 0.3 0.326229622424 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_28 (radius 1.5 ) (material helixMat28 ))
(Material helixMat29
	(color 0.725 0.275 0.827181024652 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_29 (radius 1.5 ) (material helixMat29 ))
(Material helixMat30
	(color 0.75 0.25 0.195082364552 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_30 (radius 1.5 ) (material helixMat30 ))
(Material helixMat31
	(color 0.775 0.225 0.710957939404 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_31 (radius 1.5 ) (material helixMat31 ))
(Material helixMat32
	(color 0.8 0.2 0.0768577010798 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_32 (radius 1.5 ) (material helixMat32 ))
(Material helixMat33
	(color 0.825 0.175 0.985427407793 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_33 (radius 1.5 ) (material helixMat33 ))
(Material helixMat34
	(color 0.85 0.15 0.0910406805984 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_34 (radius 1.5 ) (material helixMat34 ))
(Material helixMat35
	(color 0.875 0.125 0.986278182404 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_35 (radius 1.5 ) (material helixMat35 ))
(Material helixMat36
	(color 0.9 0.1 0.701339529727 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_36 (radius 1.5 ) (material helixMat36 ))
(Material helixMat37
	(color 0.925 0.075 0.0350832855677 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_37 (radius 1.5 ) (material helixMat37 ))
(Material helixMat38
	(color 0.95 0.05 0.795111217274 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_38 (radius 1.5 ) (material helixMat38 ))
(Material helixMat39
	(color 0.975 0.025 0.860920590413 )
	(ka 0.4 ) (kd 0.8 ) (ks 0.5 ) (ksp 50 ) (ksm 0.5 ) (kt 0 ) (ktn 1 ))
(Sphere sp_39 (radius 1.5 ) (material helixMat39 ))
(G awesome
	(I cam1 cam )
	(I amb1 amb )
	(I pt1_I pt1 (T 2 2 -45 ) )
	(I s_0 sp_0 (T 0.0 6.0 -100 ) )
	(I s_1 sp_1 (T 1.85410196625 5.70633909777 -98 ) )
	(I s_2 sp_2 (T 3.52671151375 4.85410196625 -96 ) )
	(I s_3 sp_3 (T 4.85410196625 3.52671151375 -94 ) )
	(I s_4 sp_4 (T 5.70633909777 1.85410196625 -92 ) )
	(I s_5 sp_5 (T 6.0 3.67394039744e-16 -90 ) )
	(I s_6 sp_6 (T 5.70633909777 -1.85410196625 -88 ) )
	(I s_7 sp_7 (T 4.85410196625 -3.52671151375 -86 ) )
	(I s_8 sp_8 (T 3.52671151375 -4.85410196625 -84 ) )
	(I s_9 sp_9 (T 1.85410196625 -5.70633909777 -82 ) )
	(I s_10 sp_10 (T 7.34788079488e-16 -6.0 -80 ) )
	(I s_11 sp_11 (T -1.85410196625 -5.70633909777 -78 ) )
	(I s_12 sp_12 (T -3.52671151375 -4.85410196625 -76 ) )
	(I s_13 sp_13 (T -4.85410196625 -3.52671151375 -74 ) )
	(I s_14 sp_14 (T -5.70633909777 -1.85410196625 -72 ) )
	(I s_15 sp_15 (T -6.0 -1.10218211923e-15 -70 ) )
	(I s_16 sp_16 (T -5.70633909777 1.85410196625 -68 ) )
	(I s_17 sp_17 (T -4.85410196625 3.52671151375 -66 ) )
	(I s_18 sp_18 (T -3.52671151375 4.85410196625 -64 ) )
	(I s_19 sp_19 (T -1.85410196625 5.70633909777 -62 ) )
	(I s_20 sp_20 (T -1.46957615898e-15 6.0 -60 ) )
	(I s_21 sp_21 (T 1.85410196625 5.70633909777 -58 ) )
	(I s_22 sp_22 (T 3.52671151375 4.85410196625 -56 ) )
	(I s_23 sp_23 (T 4.85410196625 3.52671151375 -54 ) )
	(I s_24 sp_24 (T 5.70633909777 1.85410196625 -52 ) )
	(I s_25 sp_25 (T 6.0 1.83697019872e-15 -50 ) )
	(I s_26 sp_26 (T 5.70633909777 -1.85410196625 -48 ) )
	(I s_27 sp_27 (T 4.85410196625 -3.52671151375 -46 ) )
	(I s_28 sp_28 (T 3.52671151375 -4.85410196625 -44 ) )
	(I s_29 sp_29 (T 1.85410196625 -5.70633909777 -42 ) )
	(I s_30 sp_30 (T 2.20436423847e-15 -6.0 -40 ) )
	(I s_31 sp_31 (T -1.85410196625 -5.70633909777 -38 ) )
	(I s_32 sp_32 (T -3.52671151375 -4.85410196625 -36 ) )
	(I s_33 sp_33 (T -4.85410196625 -3.52671151375 -34 ) )
	(I s_34 sp_34 (T -5.70633909777 -1.85410196625 -32 ) )
	(I s_35 sp_35 (T -6.0 -2.57175827821e-15 -30 ) )
	(I s_36 sp_36 (T -5.70633909777 1.85410196625 -28 ) )
	(I s_37 sp_37 (T -4.85410196625 3.52671151375 -26 ) )
	(I s_38 sp_38 (T -3.52671151375 4.85410196625 -24 ) )
	(I s_39 sp_39 (T -1.85410196625 5.70633909777 -22 ) )
	(I spGlass_I spGlass (T -2.6 -2.6 -52 ) )
	(I spB_I spWALL (T 0 -100009.0 -3 ) )
	(I spT_I spWALL (T 0 100009.0 -3 ) )
	(I spF_I spWALL (T 0 0 -100100.0 ) )
	(I spN_I spWALL (T 0 0 100000.0 ) )
	(I spL_I spLEFT (T -100009.0 0 -3 ) )
	(I spR_I spRIGHT (T 100009.0 0 -3 ) )
)
(Render awesome )
