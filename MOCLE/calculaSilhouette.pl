#! /usr/bin/perl

# teste com o coef. de corr. Spearman
# perl silhouetteVersao2.pl spearman cgapCancer.txt SOTAeuclidean-k9.clu res.txt

if (($ARGV[0] eq "") or ($ARGV[1] eq "")) {
    die ("Forma: silhouetteVersao2.pl [pearson/euclidean/spearman][arq. dados] [arq. clusters] [arq. resultado] \n" );
}

$sim = $ARGV[0];
$arqEntrada = $ARGV[1];
$arqClusters = $ARGV[2];
$arqSaida = $ARGV[3];

$name = $arqSaida;
$name =~ /(.*)\.txt/;
$arqResumo = "$1_summary.txt";
#print $arqResumo;

    # le arquivo dos clusters

    open(CLUS, "$arqClusters") || die "cannot open $arqClusters for reading: $!";
    $nClusters = 0;
    @labels = ();
    while (<CLUS>)
    {
        $line = $_;
        $line =~ /(.*)\t(\d*)/;
        $clu = $2;
        $ex = $1;
        $encontrou = 0;
        foreach $label (@labels)
        {
            if ($clu == $label)
            {
                $encontrou = 1;
            }    
        }    
        if ($encontrou == 0)
        {
            @labels = (@labels,$clu);
        }    
        #print "labels";
        push @{$clusters{$clu}}, $ex;
	  $guardaCluster{$ex} = $clu; 
	  
#        $clusters{$exemplo[1]} = ($clusters{$exemplo[1]}, $exemplo[0]); # $exemplo[0] é o identificador do exemplo e $exemplo[1] é o cluster a que pertence o exemplo
#        if ($clu > $nClusters)
#        {
#            $nClusters = $clu;
#        }
    }
    $nClusters = @labels;
    close(CLUS) || die "can't close $arqClusters: $!";

    foreach $cluster (@labels) 
    {
        $nElementos[$cluster] = @{$clusters{$cluster}};


	#print "$clusters{$cluster}\n";


#        print "NElem: $nElementos[$cluster]\n";
#        print "$clusters{$cluster}\n";
#        print "cluster $cluster: $nElementos[$cluster] elementos\n";
    }

open(ARQ, "$arqEntrada") || die "cannot open $arqEntrada for reading: $!";

@campos = split(/\t/, <ARQ>);
$nCampos = @campos - 1; # tira o identificador do elemento

while (<ARQ>)
{   
    @elemento = split(/\t/, $_);    
    $dados{$elemento[0]} = [@elemento[1..$nCampos]]; # $elemento[0] eh o identificador do exemplo
    
#    foreach $i (0 .. $nCampos-1)
#    {
#        print "dados $i: $dados{$elemento[0]}[$i]\n";
#    }

    $a{$elemento[0]} = 0;
	#print "$elemento[0]: $a{$elemento[0]}\n";
}

@exemplos = keys %dados;
$nExemplos = @exemplos;
    
close(ARQ) || die "can't close $arqEntrada: $!";

foreach $cluster (keys %clusters)
{
    #print "Cluster $cluster\n";
# calculo de a(i)

    if ($nElementos[$cluster] == 1)
    {
        $a{$exemplo} = 0;
    }
    else
    {
        foreach $exemplo (@{$clusters{$cluster}})
        {
        
            foreach $exemplo2 (@{$clusters{$cluster}})
            {
                if ($exemplo ne $exemplo2)
                {
                    if ($sim eq "spearman")
                    {
                        @rankExemplo = rank($nCampos, \@{$dados{$exemplo}});
#                        print "@rankExemplo\n";
                        @rankExemplo2 = rank($nCampos, \@{$dados{$exemplo2}});
#                        print "@rankExemplo2\n";
                        $similaridade = abs(correlation($nCampos, \@rankExemplo, \@rankExemplo2));
#                        print "sim: $similaridade\n\n";
                        $a{$exemplo} += $similaridade;                  
                    }
                    elsif ($sim eq "pearson")
                    {
                        $similaridade = abs(correlation($nCampos, \@{$dados{$exemplo}}, \@{$dados{$exemplo2}}));
                        $a{$exemplo} += $similaridade;                  
                        #print "correlacao $exemplo/$exemplo2: $similaridade\n";
                        #$dissimilaridade = (1 - $similaridade)/2;
                        #print "sim:$similaridade - diss: $dissimilaridade\n";
                    }
                    else
                    {
                        $dissimilaridade = euclidean($nCampos, \@{$dados{$exemplo}}, \@{$dados{$exemplo2}});
                        $a{$exemplo} += $dissimilaridade;
                        #print "euclidean $exemplo/$exemplo2: $dissimilaridade\n";

                    }        
                }
            }
            #print "$exemplo: $a{$exemplo}\n";
            $a{$exemplo} /= ($nElementos[$cluster] - 1); # é -1 porque é a media de todos os padroes exceto $exemplo
            #print "$exemplo: $a{$exemplo}\n";
        }
    }

    
# calculo de d(i, C)
    foreach $exemplo (keys %dados)
    {            
        @found = ();
        @found = grep(/\b$exemplo\b/, @{$clusters{$cluster}}); 
        $nfound = @found;
        if ($nfound == 0)
        {
            $d{$exemplo}{$cluster} = 0;
            foreach $exemplo2 (@{$clusters{$cluster}})
            {        
                if ($sim eq "spearman")
                {

                        @rankExemplo = rank($nCampos, \@{$dados{$exemplo}});
#                        print "@rankExemplo\n";
                        @rankExemplo2 = rank($nCampos, \@{$dados{$exemplo2}});
#                        print "@rankExemplo2\n";
                        $similaridade = abs(correlation($nCampos, \@rankExemplo, \@rankExemplo2));
#                        print "sim: $similaridade\n\n";


#
#                    $rankExemplo = rank($nCampos, \@{$dados{$exemplo}});
#                    $rankExemplo2 = rank($nCampos, \@{$dados{$exemplo2}});
#                    $similaridade = abs(correlation($nCampos, $rankExemplo, $rankExemplo));

                    $d{$exemplo}{$cluster} += $similaridade;
                }
                elsif ($sim eq "pearson")
                {
                    $similaridade = abs(correlation($nCampos, \@{$dados{$exemplo}}, \@{$dados{$exemplo2}}));
                    #$dissimilaridade = (1 - $similaridade)/2;
                    $d{$exemplo}{$cluster} += $similaridade;
                }
                else
                {
                    $dissimilaridade = euclidean($nCampos, \@{$dados{$exemplo}}, \@{$dados{$exemplo2}});
                    $d{$exemplo}{$cluster} += $dissimilaridade
                }    
            }            
            $d{$exemplo}{$cluster} /= $nElementos[$cluster];
            #print "$cluster - $exemplo - d: $d{$exemplo}{$cluster}\n";
        }
    }
}

open(SAI, ">$arqSaida") || die "cannot open $arqSaida for writting: $!";
open(RES, ">$arqResumo") || die "cannot open $arqResumo for writting: $!";

foreach $cluster (@labels) 
{
	$silClu{$cluster} = 0;
	#print "silhouete[$cluster]: $silClu{$cluster}\n";
}


$sAve = 0;
foreach $exemplo (keys %dados)
{
    @elementos = keys %clusters;
    									
    if ($d{$exemplo}{$elementos[0]} == undef)
	  {
        $bDissim{$exemplo} = $d{$exemplo}{$elementos[1]};
        $bSim{$exemplo} = $d{$exemplo}{$elementos[1]};
    }
    else
    {
        $bDissim{$exemplo} = $d{$exemplo}{$elementos[0]};
        $bSim{$exemplo} = $d{$exemplo}{$elementos[0]};
    }

#print "@elementos\n";
#print "$exemplo - $elementos[0] - $d{$exemplo}{$elementos[0]}\n"; 
 
 	  #print "exemplo: $exemplo\tbSim: $bSim{$exemplo}\n";
 

    foreach $cluster (@elementos)
    {
        @found = ();
        @found = grep(/\b$exemplo\b/, @{$clusters{$cluster}}); 
        $nfound = @found;

#print "$exemplo - $cluster\n";


#if ($nfound != 0) {print "exemplo: $exemplo\tfound: @found\n";}

        if ($nfound == 0) # se o exemplo nao pertence ao cluster
        {
        #print "ex: $exemplo\td: $d{$exemplo}{$cluster} - b: $bDissim{$exemplo}\n";
            if ($d{$exemplo}{$cluster} < $bDissim{$exemplo}) # minimo de d
            {
                $bDissim{$exemplo} = $d{$exemplo}{$cluster};
                #print "ex: $exemplo\tb: $bDissim{$exemplo}\n";
                
            }
            if ($d{$exemplo}{$cluster} > $bSim{$exemplo}) # maximo de d
            {
                $bSim{$exemplo} = $d{$exemplo}{$cluster};
            }
            #print "xxx --- $exemplo - $cluster - d: $d{$exemplo}{$cluster}\n"; 
        }
    }   
    # print "$exemplo - b: $b{$exemplo}\n";       
    
    if ($a{$exemplo} == 0) # a eh 0 se o cluster tem so 1 elemento e neste caso s deve ser 0;
    {
        $s{$exemplo} = 0;
    }
    elsif (($sim eq "pearson") || ($sim eq "spearman"))
    {
        if ($a{$exemplo} == $bSim{$exemplo})
        {
            $s{$exemplo} = 0;
        } 
        elsif ($a{$exemplo} > $bSim{$exemplo})
        {
            $s{$exemplo} = 1 - $bSim{$exemplo} / $a{$exemplo};
        }
        else
        {
           $s{$exemplo} = $a{$exemplo} / $bSim{$exemplo} - 1;
        }
        # print "$exemplo -- a: $a{$exemplo}, b: $bSim{$exemplo}, s:$s{$exemplo}\n";
        
    }
    elsif ($sim eq "euclidean")
    {
        if ($a{$exemplo} == $bDissim{$exemplo})
        {
            $s{$exemplo} = 0;
        } 
        elsif ($a{$exemplo} > $bDissim{$exemplo})
        {
            $s{$exemplo} = $bDissim{$exemplo} / $a{$exemplo} - 1;
        }
        else
        {
           $s{$exemplo} = 1 - $a{$exemplo} / $bDissim{$exemplo};
        }
        
    }     
       
    print SAI "$guardaCluster{$exemplo}\t$exemplo\t$s{$exemplo}\n"; 


    $sAve += $s{$exemplo};

    # calcula silhueta de cada cluster
    foreach $cluster (@elementos)
    {
        @found2 = ();
        @found2 = grep(/\b$exemplo\b/, @{$clusters{$cluster}});
        $nfound2 = @found2;

        #print "$exemplo - $cluster\n";

        if ($nfound2 > 0) # se o exemplo pertence ao cluster

        {
            $silClu{$cluster} += $s{$exemplo};
	    #print "cluster: $cluster\t sil: $silClu{$cluster}\tex: $exemplo\n";
        }
    }	

}

#print "$arqClusters\n";

$sAve /= $nExemplos;

print RES "Cluster\tSilhouette\n";
foreach $cluster (@labels) 
{
	$silClu{$cluster} /= $nElementos[$cluster];
	#print "cluster: $cluster\t tamanho: $nElementos[$cluster]\t sil: $silClu{$cluster}\tex: $exemplo\n";
	#print "tamanho: $nElementos[$cluster]\t sil: $silClu{$cluster}\tex: $exemplo\n";
	print RES "$cluster\t$silClu{$cluster}\n";
}

print RES "0\t$sAve\n";

close(SAI) || die "can't close $arqSaida: $!";
close(RES) || die "can't close $arqResumo: $!";





sub correlation
{
    local ($nCampos, $elemento, $cluster) = @_;
    local $i;
    
#    print "@{$elemento}\n";
#    print "@{$cluster}\n";
    
    
    # calcula a media e dp
    $somaElem = 0;
    $somaCluster = 0;
    $somaQuadElem = 0;
    $somaQuadCluster = 0;
    
    foreach $i (0..$nCampos - 1)
    {
        #print "i = $i:   $elemento->[$i]\n";
        #print "i = $i:   $cluster->[$i]\n";
        $somaElem += $elemento->[$i];
        $somaCluster += $cluster->[$i];
        $somaQuadElem += $elemento->[$i] ** 2;
        $somaQuadCluster += $cluster->[$i] ** 2;
    }
    $mediaElem = $somaElem / $nCampos;
    $mediaCluster = $somaCluster / $nCampos;

#    $dpElem = sqrt(($nCampos*$somaQuadElem - $somaElem ** 2) / $nCampos*($nCampos-1));
#    $dpCluster = sqrt(($nCampos*$somaQuadCluster - $somaCluster ** 2) / $nCampos*($nCampos-1));

    $somadenElem = 0;
    $somadenCluster = 0;
    foreach $i (0..$nCampos - 1)
    {
        $somadenElem += (($elemento->[$i] - $mediaElem) ** 2);
        $somadenCluster += (($cluster->[$i] - $mediaCluster) ** 2);
#        print "elem $elemento->[$i]:$somadenElem\n";
#        print "elem $cluster->[$i]: $somadenCluster\n";
    }
    #print "$somadenElem\n$somadenCluster\n";
    $denElem = sqrt($somadenElem);
    $denCluster = sqrt($somadenCluster);
    
    $soma = 0;
    foreach $i (0..$nCampos - 1)
    {
        $soma += ((($elemento->[$i] - $mediaElem) * ($cluster->[$i] - $mediaCluster)));
    }
    if ($soma == 0)
    {
        $correlation = 0;
    }
    else
    {
        $correlation = ($soma) / ($denElem * $denCluster);
    }
    return $correlation;
}


sub euclidean
{
    local ($nCampos, $elemento, $cluster) = @_;
    local $i;
    
    $soma = 0;
    foreach $i (0..$nCampos - 1)
    {
        $soma += ($elemento->[$i] - $cluster->[$i]) ** 2;
    }
    $euclidean = sqrt($soma);
    return $euclidean;
} 



sub rank
{
    local ($nCampos, $elemento) = @_;
    local $i;
    
    chomp (@{$elemento});
    
    undef @rank;
    
    @ordenado = sort { $a <=> $b; } @{$elemento};
    $numeroRepetidos = 1;
    foreach $i (0 .. $nCampos-1)
    {
        if (!defined($h{$ordenado[$i]})) # se eh a primeira vez que o valor aparece
        {
#            if ($numeroRepetidos != 0)
#            {
#                $h{$ordenado[$i-1]} = $somaRepetidos / ($numeroRepetidos + 1);
#                
#            }
            $h{$ordenado[$i]} = $i+1;
            $somaRepetidos = $i+1;
            $numeroRepetidos = 1;
        }
        else
        {
        # calcula o rank medio quando ocorre empate
            $somaRepetidos += $i+1;
            $numeroRepetidos++;            
            $h{$ordenado[$i]} = $somaRepetidos / ($numeroRepetidos);
        }
#        print "i: $i - ordenado: $ordenado[$i] - h: $h{$ordenado[$i]}\n";
        
    }

#    foreach $k (keys %h)
#    {
#        print "h{$k}: $h{$k}\n";
#    }

    foreach $i (0 .. $nCampos-1)
    {
        $rank[$i] = $h{$elemento->[$i]};
#        print "i: $i - elemento: $elemento->[$i] - $rank[$i]\n";
    }    
    
    
    undef %h;
    undef @ordenado;
    return @rank;

}


sub correlationVelho
{
    local ($nCampos, $elemento, $cluster) = @_;
    local $i;
    
    # calcula a media e dp
    $somaElem = 0;
    $somaCluster = 0;
    $somaQuadElem = 0;
    $somaQuadCluster = 0;
    
    foreach $i (0..$nCampos - 1)
    {
        $somaElem += $elemento->[$i];
        $somaCluster += $cluster->[$i];
        $somaQuadElem += $elemento->[$i] ** 2;
        $somaQuadCluster += $cluster->[$i] ** 2;
    }
    $mediaElem = $somaElem / $nCampos;
    $mediaCluster = $somaCluster / $nCampos;

    $dpElem = sqrt(($nCampos*$somaQuadElem - $somaElem ** 2) / $nCampos*($nCampos-1));
    $dpCluster = sqrt(($nCampos*$somaQuadCluster - $somaCluster ** 2) / $nCampos*($nCampos-1));

    $soma = 0;
    foreach $i (0..$nCampos - 1)
    {
        $soma += (($elemento->[$i] - $mediaElem) * ($cluster->[$i] - $mediaCluster))/$nCampos;
    }
    $correlation = 1 - ($soma) / ($dpElem * $dpCluster);
    return $correlation;
}
