#!/usr/bin/env perl
use strict; use warnings;
use Util;

sub RunWithInput
{
	my $sInput = @_ ? shift : Azzert ();
	
	printf (IndentWithTitle ($sInput, "Input"));
	
	my $fh;
	open ($fh, '>', 'TestInput.txt') or die;
	printf $fh ("%s", $sInput);
	close ($fh);
	
	my $sOutput = `"_Build/Main" <TestInput.txt`;
	printf (IndentWithTitle ($sOutput, "Output"));
	
	open ($fh, '>', 'TestOutput.txt') or die;
	printf $fh ("%s", $sOutput);
	close ($fh);
	
	return $sOutput;
}

sub array_eq
{
	my $rax = @_ ? shift : Azzert ();
	my $ray = @_ ? shift : Azzert ();
	
	if (scalar (@$rax) != scalar (@$ray))
		{ return 0; }
	
	for (my $i = 0; $i < scalar (@$rax); ++$i)
	{
		if (@$rax [$i] ne @$ray [$i])
			{ return 0; }
	}
	
	return 1;
}

sub Main
{
	{
		my @adtResults;
		{
			my $s0 = <<'END_MESSAGE';
Sleep 10000
AddSong (length 1000)
Prev
Sleep 100
Prev
Sleep 100
Prev
Sleep 100
Next
Sleep 100
Next
Sleep 100
Next
Sleep 2000
Exit
END_MESSAGE

			my $s1;
			{
				if (1)
				{
					$s1 = RunWithInput ($s0);
				}
				else
				{
					my $fh;
					open ($fh, '<', 'TestOutput.txt') or die;
					foreach my $sLine (<$fh>) { $s1 .= $sLine; }
					close ($fh);
				}
			}
			
			my @asLines = split (/^/, $s1);
			foreach my $sLine (@asLines)
			{
				chomp ($sLine);
				if ($sLine =~ m#We have started.*length (\d+)#)
				{
					printf (": %10u\n", $1);
					push (@adtResults, $1);
				}
			}
		}
		
		printf ("%s\n", IndentWithTitle (ArrayToString (\@adtResults), 'adtResults'));
		Azzert (array_eq (\@adtResults, [3000, 6000, 1000, 6000, 3000, 3000, 3000, 6000, 1000]));
	}
}

Main (@ARGV);

