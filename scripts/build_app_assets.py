import os
import json
from PIL import Image, ImageDraw

def generate_assets(output_dir, title_id="PPSA99001", title_name="IT Games"):
    sce_sys_dir = os.path.join(output_dir, "sce_sys")
    os.makedirs(sce_sys_dir, exist_ok=True)

    content_id = f"EP0001-{title_id}_00-ITGAMES000000000"

    # 1. Generate param.json for PS5
    param_json = {
        "applicationCategoryType": 0,
        "applicationDrmType": "free",
        "attribute": 0,
        "attribute2": 0,
        "attribute3": 0,
        "conceptId": "999001",
        "contentBadgeType": 1,
        "contentId": content_id,
        "contentVersion": "01.000.000",
        "downloadDataSize": 0,
        "localizedParameters": {
            "defaultLanguage": "en-US",
            "en-US": {
                "titleName": title_name
            }
        },
        "masterVersion": "01.00",
        "pubtools": {
            "creationDate": "2026-08-19 23:40:00",
            "toolVersion": "1.00"
        },
        "requiredSystemSoftwareVersion": "0x0100000000000000",
        "sdkVersion": "0x0100000000000000",
        "targetContentVersion": "01.000.000",
        "titleId": title_id,
        "userDefinedParam1": 0
    }

    param_path = os.path.join(sce_sys_dir, "param.json")
    with open(param_path, "w", encoding="utf-8") as f:
        json.dump(param_json, f, indent=2)
    print(f"Generated: {param_path}")

    # 2. Generate contentids.json
    contentids_path = os.path.join(output_dir, "contentids.json")
    with open(contentids_path, "w", encoding="utf-8") as f:
        json.dump([content_id], f, indent=2)
    print(f"Generated: {contentids_path}")

    # 3. Generate icon0.png (512x512)
    icon_size = (512, 512)
    icon = Image.new("RGBA", icon_size, (15, 18, 30, 255))
    draw = ImageDraw.Draw(icon)

    for r in range(230, 0, -1):
        color = (
            int(30 + (225 - 30) * (230 - r) / 230 * 0.2),
            int(80 + (240 - 80) * (230 - r) / 230 * 0.4),
            int(220 + (255 - 220) * (230 - r) / 230 * 0.6),
            255
        )
        draw.ellipse([256 - r, 256 - r, 256 + r, 256 + r], fill=color)

    draw.rounded_rectangle([90, 140, 422, 372], radius=32, fill=(20, 25, 45, 240), outline=(0, 200, 255, 255), width=4)
    draw.text((256, 200), "IT GAMES", fill=(255, 255, 255), anchor="mm")
    draw.text((256, 250), f"PS5 ({title_id})", fill=(0, 210, 255), anchor="mm")
    draw.text((256, 300), "v1.0.0", fill=(160, 180, 210), anchor="mm")

    icon_path = os.path.join(sce_sys_dir, "icon0.png")
    icon.save(icon_path, "PNG")
    print(f"Generated: {icon_path}")

    # 4. Generate pic0.png / pic1.png (1920x1080 Splash)
    pic_size = (1920, 1080)
    pic = Image.new("RGBA", pic_size, (10, 12, 22, 255))
    pic_draw = ImageDraw.Draw(pic)

    pic_draw.ellipse([700, 300, 1220, 820], fill=(20, 50, 120, 255))
    pic_draw.rounded_rectangle([560, 340, 1360, 740], radius=40, fill=(18, 22, 38, 230), outline=(0, 180, 255, 255), width=5)
    pic_draw.text((960, 480), "IT GAMES", fill=(255, 255, 255), anchor="mm")
    pic_draw.text((960, 560), f"Native PS5 Application [{title_id}]", fill=(0, 200, 255), anchor="mm")
    pic_draw.text((960, 630), "Standalone Homebrew Architecture", fill=(160, 180, 210), anchor="mm")

    pic0_path = os.path.join(sce_sys_dir, "pic0.png")
    pic.save(pic0_path, "PNG")
    print(f"Generated: {pic0_path}")

    pic1_path = os.path.join(sce_sys_dir, "pic1.png")
    pic.save(pic1_path, "PNG")
    print(f"Generated: {pic1_path}")

if __name__ == "__main__":
    import sys
    target = sys.argv[1] if len(sys.argv) > 1 else "app_pkg/PPSA99901-app0"
    generate_assets(target, "PPSA99901", "IT Games")
