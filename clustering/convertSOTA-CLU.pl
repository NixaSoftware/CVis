if (($ARGV[0] eq "") or ($ARGV[1] eq "")) {
    die ("Forma: normaliza.pl [arq. original] [arq. normalizado] \n" );
}

$arqEntrada = $ARGV[0];
$arqSaida = $ARGV[1];

$correspondence = "$arqSaida.correspondence";

open(ENT, "$arqEntrada") || die "cannot open $arqEntrada for reading: $!";
open(SAI, ">$arqSaida") || die "cannot open $arqSaida for reading: $!";

open(CURR, ">$correspondence ") || die "cannot open $correspondence for reading: $!";

$nCluster = 0;

undef %cluster;

while (<ENT>)
{
    if ($_ =~ /(.+) goes to (\d+)/)
    {
        $exemplo = $1;
        $no = $2;
#        print "$exemplo: $no\n";
        if (!defined($cluster{$no}))
        {
            $nCluster++;
            $cluster{$no} = $nCluster;
		print CURR "$nCluster\t$no\n";
        }
#        print "Cluster:$exemplo\t$cluster{$no}\n";
        print SAI "$exemplo\t$cluster{$no}\n";	  
    }
}

close(ENT) || die "can't close $arqEntrada: $!";
close(SAI) || die "can't close $arqSaida: $!";
close(CURR) || die "can't close $correspondence: $!";


