# fix_makefile_indents.py
import io, re, sys, os

INFN = "Makefile"
OUTFN = "Makefile.fixed"

with io.open(INFN, "rb") as f:
    raw = f.read()

# 1) CRLF -> LF, 유니코드 공백/제로폭 제거
text = raw.decode("utf-8", errors="replace")
text = text.replace("\r\n", "\n").replace("\r", "\n")
# zero-width spaces 제거
text = text.replace("\u200b","").replace("\ufeff","").replace("\u00a0"," ")

lines = text.split("\n")
out = []
prev_ended_with_colon = False

rule_re = re.compile(r"^\s*([^\s#][^:=]*):(?:\s|$)")  # 타깃: 뒤에 : 로 끝나는 줄
recipe_like_re = re.compile(r"^[ \t]+(?:@?[\w./$-])")  # 탭/스페이스로 시작 + 명령문 형태

for i, line in enumerate(lines, 1):
    L = line

    # 규칙 라인인지 체크 (타깃:)
    if rule_re.match(L):
        out.append(L.rstrip())
        prev_ended_with_colon = True
        continue

    # 빈 줄/주석 라인은 그대로
    if not L.strip() or L.lstrip().startswith("#"):
        out.append(L.rstrip())
        prev_ended_with_colon = False
        continue

    # 레시피 줄인데 탭 아닌 공백으로 시작하면 TAB로 교체
    if prev_ended_with_colon:
        # 이미 탭으로 시작하면 놔둠
        if L.startswith("\t"):
            out.append(L.rstrip())
        else:
            # 선행 공백 제거 후 탭 1개 + 나머지
            out.append("\t" + L.lstrip())
        # 같은 타깃의 다음 줄도 레시피일 수 있으니, prev 유지
        # 다음 줄이 또 레시피면 계속 탭
        continue

    # 이전 줄이 타깃이 아니더라도, “들여쓰기 + 명령”처럼 보이면 탭 보정
    if recipe_like_re.match(L) and not L.startswith("\t"):
        out.append("\t" + L.lstrip())
        prev_ended_with_colon = False
        continue

    out.append(L.rstrip())
    prev_ended_with_colon = False

fixed = "\n".join(out) + "\n"

with io.open(OUTFN, "w", encoding="utf-8", newline="\n") as f:
    f.write(fixed)

print(f"[fix] wrote {OUTFN}")
