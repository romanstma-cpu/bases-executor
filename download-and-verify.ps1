# download-and-verify.ps1
# Run after the GitHub Action completes to get the built exe
param([string]$Repo = "romanstma-cpu/bases-executor", [string]$Token)

# 1. List latest successful artifact
$headers = @{}
if ($Token) { $headers["Authorization"] = "token $Token" }

Write-Host "Fetching latest artifact..."
$urls = @("https://api.github.com/repos/$Repo/actions/artifacts?per_page=5")
$art = (Invoke-RestMethod -Uri $urls[0] -Headers $headers -Method GET).artifacts

$target = $art | Where-Object { $_.name -eq "bases-executor-bin" } | Sort-Object updated_at -Descending | Select-Object -First 1
if (!$target) { Write-Error "Artifact not found"; exit 1 }

Write-Host "Downloading $($target.name) (#$($target.id))..."
$dl = $target.archive_download_url
$out = "$env:TEMP\bases-executor.zip"
Invoke-RestMethod -Uri $dl -OutFile $out -Headers $headers -Method GET

Write-Host "Extracting..."
Expand-Archive -Path $out -DestinationPath ".\" -Force

# 2. Verify structure
Write-Host "`n=== VERIFICATION ==="
Write-Host "Files in zip:"
Get-ChildItem -Recurse | % { "  $_" }

# 3. Check if exe is there
$exe = Get-ChildItem -Recurse -Filter "*.exe" -ErrorAction SilentlyContinue
if ($exe) {
    Write-Host "`n✓ EXTRACTED EXE: $($exe.FullName)"
    $size = (Get-Item $exe.FullName).Length / 1MB
    Write-Host "Size: $([math]::Round($size,2)) MB"
} else {
    Write-Host "⚠ No .exe found – build may have failed silently"
}

# 4. Check test_sunc.lua is near
$lua = Get-ChildItem -Recurse -Filter "test_sunc.lua" -ErrorAction SilentlyContinue
if ($lua) { Write-Host "✓ test_sunc.lua present" }

Write-Host "`nDone. Run .\\bases_executor.exe to test sUNC."
# end