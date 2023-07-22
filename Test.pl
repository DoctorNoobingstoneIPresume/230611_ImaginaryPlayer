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

sub EscapeArg
{
	my $sArg0 = @_ ? shift : Azzert ();
	
	my $sRet = '';
	{
		for (my $ic = 0; $ic < length ($sArg0); ++$ic)
		{
			my $c = substr ($sArg0, $ic, 1);
			if ($c eq '"')
				{ $sRet .= "\\\""; }
			else
				{ $sRet .= $c; }
		}
	}
	
	return $sRet;
}

sub RunWithCommands
{
	my $rasCommands = @_ ? shift : Azzert ();
	
	printf (IndentWithTitle (ArrayToString ($rasCommands), "Commands"));
	
	my @asArgs;
	{
		foreach my $sCommand (@$rasCommands)
		{
			push (@asArgs, '-ex');
			push (@asArgs, $sCommand);
		}
	}
	
	# [2023-07-19]
	#my $sOutput = system ("_Build/Main", @asArgs);
	my $sCmdLine = "_Build/Main";
	{
		foreach my $sArg0 (@asArgs)
		{
			my $sArg1 = EscapeArg ($sArg0);
			{
				if (index ($sArg1, ' ') >= 0)
					{ $sArg1 = "\"${sArg1}\""; }
			}
			
			$sCmdLine .= " ${sArg1}";
		}
		
		$sCmdLine .= " 2>&1 | tee TestOutput.txt";
	}
	
	my $sOutput = `${sCmdLine}`;
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
	if (0)
	{
		my @adtResults;
		{
			my $s0 = <<'END_MESSAGE';
Sleep 10000
AddSong (length 1000)
Sleep 100
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
			
			my @asCommands =
			(
				'Sleep 10000',
				'AddSong (length 1000)', 'Sleep 100',
				'Prev'                 , 'Sleep 100',
				'Prev'                 , 'Sleep 100',
				'Prev'                 , 'Sleep 100',
				'Next'                 , 'Sleep 100',
				'Next'                 , 'Sleep 100',
				'Next'                 , 'Sleep 100',
				'Sleep 2000',
				'Exit'
			);
			
			my $s1;
			{
				if (0)
				{
					$s1 = RunWithInput ($s0);
				}
				elsif (1)
				{
					$s1 = RunWithCommands (\@asCommands);
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
		Azzert (array_eq (\@adtResults, [3000, 6000, 1000, 6000, 3000, 3000, 6000, 1000]));
	}
	
	{
		my $bSuccess = 0;
		{
			my $ni = 100;
			my $nj =   4;
			my @asCommands;
			{
				for (my $i = 0; $i < $ni; ++$i)
				{
					for (my $j = 0; $j < $nj; ++$j)
					{
						push (@asCommands, sprintf ('AddSong (artist Artist_%04u)', $i));
					}
				}
				
				push (@asCommands, ('RemoveDups', 'ShowSongs', 'Exit'));
			}
			
			my $s1 = RunWithCommands (\@asCommands);
			my @asLines = split (/^/, $s1);
			
			foreach my $sLine (@asLines)
			{
				chomp ($sLine);
				
				#printf (": %s\n", $sLine);
				
				if ($sLine =~ m#^\s*Number of songs.*: (\d+) - (\d+) = (\d+)[.]$#)
				{
					printf (": %u - %u = %u.\n", $1, $2, $3);
					if ($2 == $ni * ($nj - 1))
						{ $bSuccess = 1; }
				}
			}
		}
		
		Azzert ($bSuccess);
	}
}

Main (@ARGV);

